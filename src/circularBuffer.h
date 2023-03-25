/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File: circularBuffer.h
 * Author: Enrico Simetti, Pascal Quach
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef CIRCULAR_BUFFER_H
#define	CIRCULAR_BUFFER_H

#define CB_SIZE 100

/**
 * Struct: circular_buffer_t
 * -------------------------------------------------
 * 
 * @member read_index
 * @member write_index
 * @member buffer_size
 * @member buffer flexible array
 */
struct circular_buffer_t {
    int read_index;
    int write_index;
    int buffer_size;
    char buffer[];
};
typedef struct circular_buffer_t circularBuffer;

/**
 * Function: write_cb
 * ---------------------------------
 * Write a byte to circular buffer
 * Discard oldest byte if buffer is full
 * 
 * @param cb
 * @param byte
 */
void cb_write(volatile circularBuffer *cb, char byte);

/**
 * Function: read_cb
 * -----------------------
 * Read a byte from circular buffer,
 * only if buffer is not empty.
 * 
 * @param cb
 * @param byte
 */
void cb_read(volatile circularBuffer *cb, char *byte);

/**
 * Function: number_bytes_cb
 * -----------------------
 * Compute current written bytes in circular buffer
 *
 * @param cb
 * @param length
 */
void cb_number_bytes(volatile circularBuffer* cb, int *length);

/**
 * Function: cb_configuration
 * ---------------------------------------
 * Initialize circular buffer
 * @param cb
 * @param size
 */
void cb_configuration(volatile circularBuffer *cb, int size);

#endif	/* CIRCULAR_BUFFER_H */

