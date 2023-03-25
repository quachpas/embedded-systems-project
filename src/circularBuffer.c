/*
 * File:   circularBuffer.c
 * Author: Enrico Simetti, Pascal Quach
 *
 * Created on January 4, 2022, 6:26 PM
 */


#include "circularBuffer.h"
#include "Interrupt.h"

void cb_write(volatile circularBuffer *cb, char byte) {
    cb->buffer[cb->write_index] = byte;
    cb->write_index = (cb->write_index + 1) % cb->buffer_size;
    if (cb->read_index == cb->write_index) { // Check buffer full
        cb->read_index++; // discard the oldest byte
    }
}

void cb_read(volatile circularBuffer *cb, char *byte) {
    U2RXIE = 0; // Disable RX interrupt
    if (cb->read_index != cb->write_index) { // Check buffer not empty
        *byte = cb->buffer[cb->read_index];
        cb->read_index = (cb->read_index + 1) % cb->buffer_size;
    }
    U2RXIE = 1; // Enable RX interrupt
}

void cb_number_bytes(volatile circularBuffer* cb, int *length) {
    U2RXIE = 0; // Disable RX interrupt
    if (cb->read_index <= cb->write_index) {
        *length = cb->write_index - cb->read_index;
    } else {
        *length = cb->buffer_size - cb->read_index + cb->write_index;
    }
    U2RXIE = 1; // Enable RX interrupt
}

void cb_configuration(volatile circularBuffer *cb, int size) {
    cb->buffer_size = size;
    cb->read_index = 0;
    cb->write_index = 0;
}