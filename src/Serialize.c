/*
 * File:   Serialize.c
 * Author: quachpas
 *
 * Created on January 7, 2022, 7:21 PM
 */

#include <string.h>
#include <stdio.h>
#include "Controller.h"
#include "Serialize.h"
#include "Messages.h"
#include "SPI.h"
#include "fbkLCD.h"

void __serialize_msg(Message *msg, char str[MSG_LENGTH]) {
    strcat(strcat(strcat(strcat(strcat(strcpy(
            str, DOLLAR), msg->cat), msg->type), COMMA), msg->payload), STAR);
}

char* __serialize_unsigned_double(double *d, char *str) {
    sprintf(str, "%.1f", *d);
    return str;
}

char* __serialize_signed_integer(int *n, char *str) {
    if (*n < 0) {
        sprintf(str, "%d", *n);
    } else {
        sprintf(str, "+%d", *n);
    }
    return str;
}

char* __serialize_unsigned_integer(int *n, char *str) {
    if (*n < 0) {
        strcpy(str, SERIALIZE_ERROR);
    } else {
        sprintf(str, "%d", *n);
    }
    return str;
}

char* __serialize_controller_state(int *state, char *str) {
    if (*state == MODE_NORMAL) {
        strcpy(str, S_MODE_NORMAL);
    } else if (*state == MODE_TIMEOUT) {
        strcpy(str, S_MODE_TIMEOUT);
    } else if (*state == MODE_SAFE) {
        strcpy(str, S_MODE_SAFE);
    } else {
        strcpy(str, SERIALIZE_ERROR);
    }

    return str;
}

void __serialize_payload_fbk(int *n1, int *n2, int *state, char str[MSG_PAYLOAD_LENGTH]) {
    char tmp_n1[7];
    char tmp_n2[7];
    char tmp_state[2];
    __serialize_signed_integer(n1, tmp_n1);
    __serialize_signed_integer(n2, tmp_n2);
    __serialize_unsigned_integer(state, tmp_state);
    strcat(strcat(strcat(strcat(strcpy(
            str, tmp_n1), COMMA), tmp_n2), COMMA), tmp_state);
}

void __serialize_payload_tem(double *temp, char str[MSG_PAYLOAD_LENGTH]) {
    char tmp[5];
    __serialize_unsigned_double(temp, tmp);
    strcpy(
            str, tmp);
}

void __serialize_payload_ack(char msg_type[4], int *value, char str[MSG_PAYLOAD_LENGTH]) {
    char tmp[2];
    __serialize_unsigned_integer(value, tmp);
    strcat(strcat(strcpy(
            str, msg_type), COMMA), tmp);
}

void __serialize_fbk_lcd_p1r1(int *state, double *temp, char str[LCD_ROW_LENGTH + 1]) {
    char tmp_state[2];
    char tmp_temp[5];
    __serialize_controller_state(state, tmp_state);
    __serialize_unsigned_double(temp, tmp_temp);
    strcat(strcat(strcat(strcat(strcat(strcat(strcpy(
            str, ST), COLON), tmp_state), SEMI_COLON), T), COLON), tmp_temp);
}

void __serialize_fbk_lcd_p1r2(int *n1, int *n2, char str[LCD_ROW_LENGTH + 1]) {
    char tmp_n1[7];
    char tmp_n2[7];
    __serialize_unsigned_integer(n1, tmp_n1);
    __serialize_unsigned_integer(n2, tmp_n2);
    strcat(strcat(strcat(strcat(strcpy(
            str, R), COLON), tmp_n1), SEMI_COLON), tmp_n2);
}

void __serialize_fbk_lcd_p2r1(int *rpm_max, int *rpm_min, char str[LCD_ROW_LENGTH + 1]) {
    char tmp_rpm_max[7];
    char tmp_rpm_min[7];
    __serialize_signed_integer(rpm_max, tmp_rpm_max);
    __serialize_signed_integer(rpm_min, tmp_rpm_min);

    strcat(strcat(strcat(strcat(strcpy(
            str, SA), COLON), tmp_rpm_min), SEMI_COLON), tmp_rpm_max);
}

void __serialize_fbk_lcd_p2r2(double *dc1, double *dc2, char str[LCD_ROW_LENGTH + 1]) {
    char tmp_dc1[5];
    char tmp_dc2[5];
    __serialize_unsigned_double(dc1, tmp_dc1);
    __serialize_unsigned_double(dc2, tmp_dc2);
    strcat(strcat(strcat(strcat(strcpy(
            str, R), COLON), tmp_dc1), SEMI_COLON), tmp_dc2);
}