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
 * File:   Interrupt.h  
 * Author: Pascal Quach <pascal.quach@etu.utc.fr>
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef INTERRUPT_H
#define	INTERRUPT_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "Timer.h"

// INT 0
#define INT0IF IFS0bits.INT0IF
#define INT0IE IEC0bits.INT0IE
#define INT1IE IEC1bits.INT1IE

// TIMER 1
#define T1IF IFS0bits.T1IF
#define T1IE IEC0bits.T1IE

// TIMER 2
#define T2IF IFS0bits.T2IF
#define T2IE IEC0bits.T2IE

// TIMER 3
#define T3IF IFS0bits.T3IF
#define T3IE IEC0bits.T3IE

// TIMER 4
#define T4IF IFS1bits.T4IF
#define T4IE IEC1bits.T4IE

// TIMER 5
#define T5IF IFS1bits.T5IF
#define T5IE IEC1bits.T5IE

// U2RX
#define U2RXIE IEC1bits.U2RXIE
#define U2RXIF IFS1bits.U2RXIF
#define U2TXIE IEC1bits.U2TXIE
#define U2TXIF IFS1bits.U2TXIF

/**
 * Function: int_enable_disable_timer_interrupt
 * ---------------------------
 * Enable interrupt in IEC registers
 * 0 = interrupt request disabled
 * 1 = interrupt request enabled
 * 
 * @param t pointer to timer
 * @param txie txie boolean
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int int_enable_disable_timer_interrupt(Timer *t, int txie);

/**
 * ISR: _T2Interrupt
 * ------------------------------------------
 * Interrupt handler for T2, timeout timer.
 * - Stop and reset TMR
 * - Update mtr references to 0
 *
 */
void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt();


/**
 * ISR: _U2RXInterrupt()
 * ---------------------------------------------------
 * Interrupt handler for UART2 Receiver
 * - Write to circular buffer from U2RXREG
 * - Discard oldest byte
 */
void __attribute__((__interrupt__, __auto_psv__)) _U2RXInterrupt();

/**
 * ISR: _OscillatorFail
 * ----------------------
 * For debugging purposes
 */
void __attribute__((interrupt, no_auto_psv)) _OscillatorFail();

/**
 * ISR: _OscillatorFail
 * ----------------------
 * For debugging purposes
 */
void __attribute__((interrupt, auto_psv)) _AddressError();

/**
 * ISR: _OscillatorFail
 * ----------------------
 * For debugging purposes
 */
void __attribute__((interrupt, no_auto_psv)) _StackError();

/**
 * ISR: _OscillatorFail
 * ----------------------
 * For debugging purposes
 */
void __attribute__((interrupt, no_auto_psv)) _MathError();
#endif	/* INTERRUPT_H */

