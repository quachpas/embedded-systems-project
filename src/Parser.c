/*
 * File:   Parser.c
 * Author: Enrico Simetti, Pascal Quach
 *
 */

// std
#include <string.h>
#include "Interrupt.h"
#include "Motor.h"
#include "circularBuffer.h"
#include "Parser.h"
#include "Controller.h"
#include "extern.h"

void parser_state_configuration(parserState *ps) {
    ps->state = STATE_DOLLAR;
    ps->index_cat = 0;
    ps->index_type = 0;
    ps->index_payload = 0;
}

int __parse_byte(parserState *ps, char byte) {
    switch (ps->state) {
        case STATE_DOLLAR:
            if (byte == '$') {
                ps->state = STATE_CAT;
                ps->index_cat = 0;
            }
            break;
        case STATE_CAT:
            if (strcmp(ps->msg.cat, HL) == 0) {
                ps->state = STATE_TYPE;
                ps->msg.cat[ps->index_cat] = '\0';
                ps->index_type = 0;
                ps->msg.type[ps->index_type] = byte; // Don't lose current byte
                ps->index_type++;
            } else if (byte == ',' || byte == '*' || ps->index_cat > MSG_CAT_LENGTH) { // error! 
                ps->state = STATE_DOLLAR;
                ps->index_cat = 0;
                return PARSE_ERROR;
            } else {
                ps->msg.cat[ps->index_cat] = byte; // ok!
                ps->index_cat++; // increment for the next time;
            }
            break;
        case STATE_TYPE:
            if (byte == ',' && ps->index_type == 3) {
                ps->state = STATE_PAYLOAD;
                ps->msg.type[ps->index_type] = '\0';
                ps->index_payload = 0; // initialize properly the index
            } else if (ps->index_type > MSG_TYPE_LENGTH) { // error! 
                ps->state = STATE_DOLLAR;
                ps->index_type = 0;
                return PARSE_ERROR;
            } else if (byte == '*') {
                ps->state = STATE_DOLLAR; // get ready for a new message
                ps->msg.type[ps->index_type] = '\0';
                ps->msg.payload[0] = '\0'; // no payload
                return NEW_MESSAGE;
            } else {
                ps->msg.type[ps->index_type] = byte; // ok!
                ps->index_type++; // increment for the next time;
            }
            break;
        case STATE_PAYLOAD:
            if (byte == '*') {
                ps->state = STATE_DOLLAR; // get ready for a new message
                ps->msg.payload[ps->index_payload] = '\0';
                return NEW_MESSAGE;
            } else if (ps->index_payload > MSG_PAYLOAD_LENGTH) { // error
                ps->state = STATE_DOLLAR;
                ps->index_payload = 0;
                return PARSE_ERROR;
            } else {
                ps->msg.payload[ps->index_payload] = byte; // ok!
                ps->index_payload++; // increment for the next time;
            }
            break;
    }
    return NO_MESSAGE;
}

int __parse_signed_integer(char const *str, int *index, int *n) {
    int number = 0;
    int sign = 1;

    // Sign
    if (str[*index] == '-') {
        sign = -1;
        (*index)++;
    } else if (str[*index] == '+') {
        sign = 1;
        (*index)++;
    }

    // Integer
    while (str[*index] != ',' && str[*index] != '\0') {
        if ((str[*index] - '0') < 0 || (str[*index] - '0') > 9)
            return PARSE_ERROR;
        number *= 10; // multiply the current number by 10;
        number += str[*index] - '0'; // converting character to decimal number
        (*index)++;
    }

    *n = sign * number;

    return PARSE_SUCCESS;
}

int __parse_n_signed_integers(const char *str, int n, int arr[n]) {
    int ret = 1;
    int index = 0;

    int i;
    for (i = 0; i < n; i++) {
        ret += __parse_signed_integer(str, &index, &(arr[i]));
        index++;
    }

    if (ret < 0) {
        return PARSE_ERROR;
    } else {
        return PARSE_SUCCESS;
    }
};

int __parse_hlref(struct controller_t *ctrl, parserState *ps) {
    // HLREF
    int refs[N_MOTORS];

    if (__parse_n_signed_integers(ps->msg.payload, N_MOTORS, refs)) {
        return PARSE_ERROR;
    } else {
        mtr_update_refs(&ctrl->mtr, refs);
    };

    return PARSE_SUCCESS;
}

int __parse_hlsat(struct controller_t *ctrl, parserState *ps) {
    // HLSAT
    int sats_arr[2];

    if (__parse_n_signed_integers(ps->msg.payload, 2, sats_arr)) {
        return PARSE_ERROR;
    } else {
        // Struct
        Saturations sats;
        sats.rpm_min = sats_arr[0];
        sats.rpm_max = sats_arr[1];

        // Check values
        if (__mtr_check_sats(&sats)) {
            return PARSE_ERROR;
        }

        // Update values
        mtr_update_sats(&ctrl->mtr, &sats);
    };

    return PARSE_SUCCESS;
}

int __parse_hlena(struct controller_t *ctrl, parserState *ps) {
    return PARSE_SUCCESS;
}

int parse_message(struct controller_t *ctrl, parserState *ps) {
    char byte;

    cb_read(cb_rxreg, &byte);

    int ret = __parse_byte(ps, byte);
    if (ret == NEW_MESSAGE) {
        if (strcmp(ps->msg.cat, HL) == 0) {
            // Received messages
            if (strcmp(ps->msg.type, REF) == 0) {
                // Reset timeout period
                Timer tmr_timeout;
                tmr_get(&tmr_timeout, TIMER_TIMEOUT, 1);
                // Parse
                if (ctrl->mode == MODE_NORMAL) {
                    *tmr_timeout.TMRX = 0;
                    if (__parse_hlref(ctrl, ps)) {
                        return PARSE_ERROR;
                    }
                } else if (ctrl->mode == MODE_TIMEOUT) {
                    __ctrl_change_mode(ctrl, MODE_NORMAL);
                    if (__parse_hlref(ctrl, ps)) {
                        return PARSE_ERROR;
                    }
                    tmr_start_stop(&tmr_timeout, 1); // Start timer
                }
            } else if (strcmp(ps->msg.type, SAT) == 0) {
                if (__parse_hlsat(ctrl, ps)) {
                    msg_send_ack(SAT, _NACK);
                    return PARSE_ERROR;
                } else {
                    msg_send_ack(SAT, _ACK);
                }
            } else if (strcmp(ps->msg.type, ENA) == 0) {
                if (ctrl->mode == MODE_SAFE) {
                    if (__parse_hlena(ctrl, ps)) {
                        return PARSE_ERROR;
                    } else {
                        __ctrl_change_mode(ctrl, MODE_NORMAL);
                    }
                }
            }
        } else {
            return PARSE_ERROR;
        }
    } else if (ret == PARSE_ERROR) {
        return PARSE_ERROR;
    } else if (ret == NO_MESSAGE) {
        return NO_MESSAGE;
    } else {
        return PARSE_ERROR;
    }

    return PARSE_SUCCESS;
}

