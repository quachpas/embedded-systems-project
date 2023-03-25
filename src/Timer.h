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
 * File:  TIMER.h
 * Author: Pascal Quach <pascal.quach@etu.utc.fr>
 * Comments:
 * Revision history: 
 * Created on October 5, 2021, 2:40 PM
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef TIMER_H
#define	TIMER_H

#define TIMER_MAIN_LOOP 1
#define TIMER_TIMEOUT 2
#define TIMER3 3
#define TIMER4 4
#define TIMER5 5
#define T1TON T1CONbits.TON
#define T2TON T2CONbits.TON 
#define T3TON T3CONbits.TON 
#define T4TON T4CONbits.TON 
#define T5TON T5CONbits.TON 
#define T1CKPS T1CONbits.TCKPS
#define T2CKPS T2CONbits.TCKPS 
#define T3CKPS T3CONbits.TCKPS 
#define T4CKPS T4CONbits.TCKPS 
#define T5CKPS T5CONbits.TCKPS 
#define T1TCS T1CONbits.TCS
#define T2TCS T2CONbits.TCS 
#define T3TCS T3CONbits.TCS 
#define T4TCS T4CONbits.TCS 
#define T5TCS T5CONbits.TCS 
#define T1TGATE T1CONbits.TGATE
#define T2TGATE T2CONbits.TGATE 
#define T3TGATE T3CONbits.TGATE 
#define T4TGATE T4CONbits.TGATE 
#define T5TGATE T5CONbits.TGATE 


#include <stdint.h>

struct timer_t {
    int number; // Timer number
    int isr_enabled; // ISR is default or custom
    volatile uint16_t *TMRX; // Timer value
    volatile uint16_t *PRX; // Period
};
typedef struct timer_t Timer;

/**
 * Function: __tmr_compute_ticks_not_ps
 * -------------------------------
 * Computes the number of clock instructions elapsed during the given duration in ms
 * @param ms duration in ms
 * @param ticks_not_ps pointer to long int where to store the value
 */
void __tmr_compute_ticks_not_ps(int ms, long *ticks_not_ps);

/**
 * Function: __tmr_compute_prescaler
 * ----------------------------
 * Compute prescaler ratio according to the number of clock instructions
 * @param ticks_not_ps pointer to number of clock instructions not prescaled
 * @param tckps pointer to prescaler integer
 * @return int EXIT_SUCCESS or EXIT_FAILURE
 */
int __tmr_compute_prescaler(long *ticks_not_ps, int *tckps);

/**
 * Function: __tmr_prescale_ticks
 * ----------------------------
 * Prescale clock instructions
 * @param tckps prescaler
 * @param ticks_not_ps pointer to number of clock instructed not prescaled
 * @param ticks pointer to number of clock instructions prescaled
 * @return clock instructions
 */
long __tmr_prescale_ticks(int *tckps, long *ticks_not_ps, long *ticks);

/**
 * Function: __tmr_set_timer_prescale
 * --------------------------
 * Set the prescale
 *
 * @param t pointer to timer
 * @param tckps pointer to prescale factor
 * 0. 1:1
 * 1. 1:8
 * 2. 1:65
 * 3. 1:256
 * @return int EXIT_SUCCESS or EXIT_FAILURE
 */
int __tmr_set_timer_prescale(Timer *t, int *tckps);

/**
 * Function: tmr_start_stop
 * --------------------------
 * Start/Stop the timer
 * 
 * @param t pointer to timer
 * @param ton ton boolean
 * @return int EXIT_SUCCESS or EXIT_FAILURE
 */
int tmr_start_stop(Timer *t, int ton);

/**
 * Function: __tmr_set_clock_source
 * ---------------------------------------
 * Switch timer source
 * 0=internal clock (Fosc/4)
 * 1=External clock from pin TxCk
 * 
 * @param t pointer to timer
 * @param source integer for tcs bool
 * @return int EXIT_SUCCESS or EXIT_FAILURE
 */
int __tmr_set_clock_source(Timer *t, int tcs);

/**
 * Function: __tmr_set_gated_time_accumulation
 * ---------------------------------------
 * Enable or disable gated time accumulation
 * If timer source is external, TGATE is read as 0.
 *
 * 0=Gated time accumulation disabled
 * 1=Gated time accumulation enabled
 * 
 * @param t pointer to timer
 * @param tgate integer for tgate bool
 * @return int EXIT_SUCCESS or EXIT_FAILURE
 */
int __tmr_set_gated_time_accumulation(Timer *t, int tgate);

/**
 * Function: tmr_get
 * ------------------
 * Configure and return the timer
 * @param t pointer to Timer
 * @param number number of the timer
 * @param isr_exists boolean 
 */
void tmr_get(Timer *t, int number, int isr_exists);

/* Function: tmr_setup_period
 * ----------------------------
 * Configure and start the timer *time* to count for specified *ms*.
 * This function should support values up to one second.
 * 
 * @param t pointer to timer
 * @param ms the milliseconds duration for the timer
 * 
 */
void tmr_setup_period(Timer *t, int ms);

/*
 * Function: tmr_wait_period
 * --------------------------
 * Wait until the timer flag has expired.
 * Exits if timer is stopped or ISR exists.
 * 
 * @param t pointer to timer
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int tmr_wait_period(Timer *t);

/*
 * Function: tmr_wait_ms
 * -----------------------------
 * Configure, start and wait using the timer and specified time.
 * 
 * @param t pointer to timer
 * @param ms the milliseconds duration
 */
void tmr_wait_ms(Timer *t, int ms);

#endif	/* TIMER_H */

