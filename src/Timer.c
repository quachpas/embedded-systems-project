/*
 * File:   Timer.c
 * Author: Pascal Quach <pascal.quach@etu.utc.fr>
 *
 * Created on October 5, 2021, 2:38 PM
 */


#include <xc.h>
#include "stdlib.h"
#include "Timer.h"
#include "Interrupt.h"

void tmr_get(Timer *t, int number, int isr_exists) {
    t->number = number;
    t->isr_enabled = isr_exists;
    switch (t->number) {
        case 1:
            t->TMRX = &TMR1;
            t->PRX = &PR1;
            if (isr_exists) {
                T1IE = 1;
            }
            break;
        case 2:
            t->TMRX = &TMR2;
            t->PRX = &PR2;
            if (isr_exists) {
                T2IE = 1;
            }
            break;
        case 3:
            t->TMRX = &TMR3;
            t->PRX = &PR3;
            if (isr_exists) {
                T3IE = 1;
            }
            break;
        case 4:
            t->TMRX = &TMR4;
            t->PRX = &PR4;
            if (isr_exists) {
                T4IE = 1;
            }
            break;
        case 5:
            t->TMRX = &TMR5;
            t->PRX = &PR5;
            if (isr_exists) {
                T5IE = 1;
            }
            break;
    }
}

int tmr_start_stop(Timer *t, int ton) {
    if (ton != 0 && ton != 1) {
        // Value not in bounds
        return EXIT_FAILURE;
    }
    switch (t->number) {
        case 1:
            T1TON = ton;
            return EXIT_SUCCESS;
        case 2:
            T2TON = ton;
            return EXIT_SUCCESS;
        case 3:
            T3TON = ton;
            return EXIT_SUCCESS;
        case 4:
            T4TON = ton;
            return EXIT_SUCCESS;
        case 5:
            T5TON = ton;
            return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

int __tmr_set_timer_prescale(Timer *t, int *tckps) {
    switch (t->number) {
        case 1:
            T1CKPS = *tckps;
            return EXIT_SUCCESS;
        case 2:
            T2CKPS = *tckps;
            return EXIT_SUCCESS;
        case 3:
            T3CKPS = *tckps;
            return EXIT_SUCCESS;
        case 4:
            T4CKPS = *tckps;
            return EXIT_SUCCESS;
        case 5:
            T5CKPS = *tckps;
            return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

void __tmr_compute_ticks_not_ps(int ms, long *ticks_not_ps) {
    *ticks_not_ps = (7372800L / 4L) // Fcy = Fosc/4, Hz, s^-1
            / 1000L // -> Number of clock cycle in one ms
            * ms; // -> Number of clock cycles for the given ms
}

int __tmr_compute_prescaler(long *ticks_not_ps, int *tckps) {
    long ticks = *ticks_not_ps; // copy to reduce approximation errors

    // pre-scaler 1:1
    if (ticks <= 65535) {
        *tckps = 0;
        return EXIT_SUCCESS;
    }
    // pre-scaler 1:8
    ticks = *ticks_not_ps / 8;
    if (ticks <= 65535) {
        *tckps = 1;
        return EXIT_SUCCESS;
    }
    // pre-scaler 1:64
    ticks = *ticks_not_ps / 64;
    if (ticks <= 65535) {
        *tckps = 2;
        return EXIT_SUCCESS;
    }
    // pre-scaler 1:256
    ticks = *ticks_not_ps / 256;
    if (ticks <= 65535) {
        *tckps = 3;
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

long __tmr_prescale_ticks(int *tckps, long *ticks_not_ps, long *ticks) {
    switch (*tckps) {
        case 0:
            *ticks = *ticks_not_ps;
            return EXIT_SUCCESS;
        case 1:
            *ticks = *ticks_not_ps / 8;
            return EXIT_SUCCESS;
        case 2:
            *ticks = *ticks_not_ps / 64;
            return EXIT_SUCCESS;
        case 3:
            *ticks = *ticks_not_ps / 256;
            return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

int __tmr_set_clock_source(Timer *t, int tcs) {
    if (tcs != 0 && tcs != 1) {
        // Value not in bounds
        return EXIT_FAILURE;
    }
    switch (t->number) {
        case 1:
            T1TCS = tcs;
            return EXIT_SUCCESS;
        case 2:
            T2TCS = tcs;
            return EXIT_SUCCESS;
        case 3:
            T3TCS = tcs;
            return EXIT_SUCCESS;
        case 4:
            T4TCS = tcs;
            return EXIT_SUCCESS;
        case 5:
            T5TCS = tcs;
            return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

int __tmr_set_gated_time_accumulation(Timer *t, int tgate) {
    if (tgate != 0 && tgate != 1) {
        // Value not in bounds
        return EXIT_FAILURE;
    }
    switch (t->number) {
        case 1:
            T1TGATE = tgate;
            return EXIT_SUCCESS;
        case 2:
            T2TGATE = tgate;
            return EXIT_SUCCESS;
        case 3:
            T3TGATE = tgate;
            return EXIT_SUCCESS;
        case 4:
            T4TGATE = tgate;
            return EXIT_SUCCESS;
        case 5:
            T5TGATE = tgate;
            return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

int tmr_wait_period(Timer *t) {
    if (t->isr_enabled) {
        return EXIT_FAILURE;
    }
    switch (t->number) {
        case TIMER_MAIN_LOOP:
            if (T1TON != 1) { // if timer is stopped
                return EXIT_FAILURE;
            }
            if (T1IF == 1) { // Timer has already ended
                T1IF = 0; // Clear the interrupt flag
                *t->TMRX = 0; // Clear the timer
                tmr_start_stop(t, 0); // Stop the timer
                return EXIT_SUCCESS; // Timer already expired
            }
            while (T1IF == 0); // Wait flag timer
            T1IF = 0; // Clear the interrupt flag
            *t->TMRX = 0; // Clear the timer
            tmr_start_stop(t, 0); // Stop the timer
            return EXIT_SUCCESS;
        case TIMER_TIMEOUT:
            if (T2TON != 1) { // if timer is stopped
                return EXIT_FAILURE;
            }
            if (T2IF == 1) { // Timer is already finished.
                T2IF = 0; // Clear the interrupt flag
                *t->TMRX = 0; // Clear the timer
                tmr_start_stop(t, 0); // Stop the timer
                return EXIT_SUCCESS; // Timer already expired
            }
            while (T2IF == 0); // Wait flag timer
            T2IF = 0; // Clear the interrupt flag
            *t->TMRX = 0; // Clear the timer
            tmr_start_stop(t, 0); // Stop the timer
            return EXIT_SUCCESS;
        case TIMER3:
            if (T3TON != 1) { // if timer is stopped
                return EXIT_FAILURE;
            }
            if (T3IF == 1) { // Timer is already finished.
                T3IF = 0; // Clear the interrupt flag
                *t->TMRX = 0; // Clear the timer
                tmr_start_stop(t, 0); // Stop the timer
                return EXIT_SUCCESS; // Timer already expired
            }
            while (T3IF == 0); // Wait flag timer
            T3IF = 0; // Clear the interrupt flag
            *t->TMRX = 0; // Clear the timer
            tmr_start_stop(t, 0); // Stop the timer
            return EXIT_SUCCESS;
        case TIMER4:
            if (T4TON != 1) { // if timer is stopped
                return EXIT_FAILURE;
            }
            if (T4IF == 1) { // Timer is already finished.
                T4IF = 0; // Clear the interrupt flag
                tmr_start_stop(t, 0); // Stop the timer
                return EXIT_SUCCESS; // Timer already expired
            }
            while (T4IF == 0); // Wait flag timer
            T4IF = 0; // Clear the interrupt flag
            tmr_start_stop(t, 0); // Stop the timer
            return EXIT_SUCCESS;
        case TIMER5:
            if (T5TON != 1) { // if timer is stopped
                return EXIT_FAILURE;
            }
            if (T5IF == 1) { // Timer is already finished.
                T5IF = 0; // Clear the interrupt flag
                *t->TMRX = 0; // Clear the timer
                tmr_start_stop(t, 0); // Stop the timer
                return EXIT_SUCCESS; // Timer already expired
            }
            while (T5IF == 0); // Wait flag timer
            T5IF = 0; // Clear the interrupt flag
            *t->TMRX = 0; // Clear the timer
            tmr_start_stop(t, 0); // Stop the timer
            return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

void tmr_setup_period(Timer *t, int ms) {
    // Timer configuration
    long *ticks_not_ps = malloc(sizeof (long));
    int *tckps = malloc(sizeof (int));
    long *ticks = malloc(sizeof (long));

    __tmr_compute_ticks_not_ps(ms, ticks_not_ps);
    __tmr_compute_prescaler(ticks_not_ps, tckps);
    __tmr_prescale_ticks(tckps, ticks_not_ps, ticks);

    *t->PRX = *ticks; // Set timer period
    __tmr_set_clock_source(t, 0); // Internal clock
    __tmr_set_gated_time_accumulation(t, 0); // Disable gated time acc.
    __tmr_set_timer_prescale(t, tckps); // Set timer prescale

    // Free memory
    free(ticks);
    free(tckps);
    free(ticks_not_ps);

    // Interrupt configuration
    T1IF = 0; // Clear interrupt flag
    *t->TMRX = 0; // Reset the timer
};

void tmr_wait_ms(Timer *t, int ms) {
    tmr_setup_period(t, ms);
    tmr_start_stop(t, 1); // Start timer
    tmr_wait_period(t);
};