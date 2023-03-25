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
 * File: UART.h
 * Author: Pascal Quach <pascal.quach@etu.utc.fr>
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef UART_H
#define	UART_H

#include <xc.h>
#define UTXBF U2STAbits.UTXBF

/*
 * General Notes
 * --------------------------
 * 
 * UART2 is used because SPI pins conflict with UART1
 */

/**
 * Function: __uart2_compute_baud_rate
 * -----------------------------------
 * Compute baud rate from desired baud rate
 * 
 * @param desired_baud_rate
 * @param computed_baud_rate
 */
void __uart2_compute_baud_rate(long desired_baud_rate, long *computed_baud_rate);

/**
 * Function: uart2_configuration
 * -----------------------------
 * Enable the UART2 and configure
 * 
 * Baud Rate Divisor
 *  Desired baud rate: 9600
 *  Fcy / (16 * 9600) - 1
 * U2BRG -> 11
 * Error = 0.0
 * 
 * Stop Selection
 * STSEL -> 1 stop bit
 * 
 * Parity and Data selection
 * PDSEL -> 8 bit data, no parity
 * 
 * UART enable
 * UARTEN -> enabled
 * 
 * Transmit enable
 * UTXEN -> enabled
 * 
 */
void uart2_configuration();

/**
 * Function: uart2_write_txreg
 * ---------------------------------------------------
 * Send entire string to UART2 TX.
 * When U2TXREG is full, write the rest of the string to cb_txreg
 * in order to have a non blocking function.
 * 
 * @param string pointer to string
 */
void uart2_write_txreg(char* string);


#endif	/* UART_H */

