/*
 * File:   UART.c
 * Author: Pascal Quach <pascal.quach@etu.utc.fr>
 *
 * Created on October 26, 2021, 12:19 PM
 */

#include <xc.h>
#include "UART.h"
#include "Interrupt.h"
#include "circularBuffer.h"
#include "extern.h"

void __uart2_compute_baud_rate(long desired_baud_rate, long *computed_baud_rate) {
    *computed_baud_rate = (7372800L / 4L) / (desired_baud_rate) / 16 - 1;
}

void uart2_configuration(long desired_baud_rate) {
    // Baud rate
    long computed_baud_rate = 0;
    __uart2_compute_baud_rate(desired_baud_rate, &computed_baud_rate);
    U2BRG = computed_baud_rate;

    // 8, N, 1
    U2MODEbits.PDSEL = 0b00; // 8 bit no parity
    U2MODEbits.STSEL = 0; // 1 stop bit

    // Enable UART first
    U2MODEbits.UARTEN = 1; // UART enable
    U2STAbits.UTXEN = 1; // enable transmission
    U2STAbits.UTXISEL = 0; // ISR on word transmission
}

void uart2_write_txreg(char *string) {
    int i;
    int buffer_full = 0;
    U2TXIE = 0;
    for (i = 0; string[i] != '\0'; i++) {
        if (UTXBF == 1) {
            buffer_full = 1;
        }
        if (buffer_full) {
            cb_write(cb_txreg, string[i]);
        } else {
            U2TXREG = string[i];
        }
    }
    U2TXIE = 1;
}