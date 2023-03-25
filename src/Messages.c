/*
 * File:   Messages.c
 * Author: quachpas
 *
 * Created on January 7, 2022, 7:22 PM
 */

#include <string.h>
#include "Messages.h"
#include "circularBuffer.h"
#include "Serialize.h"
#include "UART.h"
#include "extern.h"

void msg_fbk(int *n1, int *n2, int *state, Message *msg) {
    strcpy(msg->cat, MC);
    strcpy(msg->type, FBK);
    __serialize_payload_fbk(n1, n2, state, msg->payload);
}

void msg_tem(double *temp, Message *msg) {
    strcpy(msg->cat, MC);
    strcpy(msg->type, TEM);
    __serialize_payload_tem(temp, msg->payload);
}

void msg_ack(char msg_type[4], int value, Message *msg) {
    strcpy(msg->cat, MC);
    strcpy(msg->type, ACK);
    __serialize_payload_ack(msg_type, &value, msg->payload);
}

void msg_send(Message *msg) {
    char str[MSG_LENGTH];
    __serialize_msg(msg, str);
    uart2_write_txreg(str);
}

void msg_poll(struct controller_t *ctrl, struct parserState_t *ps) {
    int avl;
    cb_number_bytes(cb_rxreg, &avl);

    int count = 0;

    while (count < avl) {
        parse_message(ctrl, ps);
        count++;
    }
}

void msg_send_ack(char msg_type[MSG_TYPE_LENGTH + 1], int value) {
    Message msg;

    msg_ack(msg_type, value, &msg);
    msg_send(&msg);
}