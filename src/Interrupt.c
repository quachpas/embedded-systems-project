/*
 * File:   Interrupt.c
 * Author: Pascal Quach <pascal.quach@etu.utc.fr>
 *
 * Created on October 14, 2021, 11:24 AM
 */


#include "Interrupt.h"
#include "Timer.h"
#include "extern.h"

void __attribute__((__interrupt__, __auto_psv__)) _U2RXInterrupt() {
    U2RXIF = 0; // clear flag

    // write_cb
    while (U2STAbits.URXDA == 1) {
        cb_rxreg->buffer[cb_rxreg->write_index] = U2RXREG;
        cb_rxreg->write_index = (cb_rxreg->write_index + 1) % cb_rxreg->buffer_size;
        if (cb_rxreg->read_index == cb_rxreg->write_index) {
            // full buffer
            cb_rxreg->read_index++; // discard the oldest byte
        }
    }
}

void __attribute__((__interrupt__, __auto_psv__)) _U2TXInterrupt() {
    U2TXIF = 0; // clear flag
    U2RXIE = 0; // Disable RX interrupt
    char byte;
    // send from cb
    while (U2STAbits.UTXBF == 0 && cb_txreg->read_index != cb_txreg->write_index) {
        // While U2TXREG not full and buffer not empty
        byte = cb_txreg->buffer[cb_txreg->read_index];
        cb_txreg->read_index = (cb_txreg->read_index + 1) % cb_txreg->buffer_size;
        U2TXREG = byte;
    }
    U2RXIE = 1; // Enable RX interrupt
}

void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt() {
    // Timeout timer
    T2IF = 0; // Clear flag
    T2TON = 0; // Stop timer
    TMR2 = 0; // Reset timer

    ctrl->mode = MODE_TIMEOUT;
    int i;
    for (i = 0; i < N_MOTORS; i++) {
        ctrl->mtr.refs[i] = 0;
    }
}

void __attribute__((interrupt, no_auto_psv)) _OscillatorFail() {
    INTCON1bits.OSCFAIL = 0;
    while (1);
}

void __attribute__((interrupt, auto_psv)) _AddressError() {
    INTCON1bits.ADDRERR = 0;
    while (1);
}

void __attribute__((interrupt, no_auto_psv)) _StackError() {
    INTCON1bits.STKERR = 0;
    while (1);
}

void __attribute__((interrupt, no_auto_psv)) _MathError() {
    INTCON1bits.MATHERR = 0;
    while (1);
}