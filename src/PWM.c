/*
 * File:   PWM.c
 * Author: Pascal Quach
 *
 * Created on November 11, 2021, 11:24 AM
 */

#include <xc.h>
#include "stdlib.h"
#include "PWM.h"

int __pwm_compute_dtaps(long *dta_not_ps, int *dtaps) {
    long dta = *dta_not_ps; // copy to reduce approximation errors

    // pre-scaler 1:1
    if (dta <= 63) {
        *dtaps = 0;
        return EXIT_SUCCESS;
    }
    // pre-scaler 1:2
    dta = *dta_not_ps / 2;
    if (dta <= 63) {
        *dtaps = 1;
        return EXIT_SUCCESS;
    }
    // pre-scaler 1:4
    dta = *dta_not_ps / 4;
    if (dta <= 63) {
        *dtaps = 2;
        return EXIT_SUCCESS;
    }
    // pre-scaler 1:8
    dta = *dta_not_ps / 8;
    if (dta <= 63) {
        *dtaps = 3;
        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}

long __pwm_prescale_dta(int *dtaps, long *dta_not_ps, long *dta) {
    switch (*dtaps) {
        case 0:
            *dta = *dta_not_ps;
            return EXIT_SUCCESS;
        case 1:
            *dta = *dta_not_ps / 2;
            return EXIT_SUCCESS;
        case 2:
            *dta = *dta_not_ps / 4;
            return EXIT_SUCCESS;
        case 3:
            *dta = *dta_not_ps / 8;
            return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

void __pwm_compute_dta_not_ps(long dead_time, long *dta_not_ps) {
    *dta_not_ps = (dead_time / 543L);
}

int __pwm_compute_ptckps(long *ptper_not_ps, int *ptckps) {
    long ptper = *ptper_not_ps; // copy to reduce approximation errors

    // pre-scaler 1:1
    if (ptper <= 32767) {
        *ptckps = 0;
        return EXIT_SUCCESS;
    }
    // pre-scaler 1:4
    ptper = *ptper_not_ps / 4;
    if (ptper <= 32767) {
        *ptckps = 1;
        return EXIT_SUCCESS;
    }
    // pre-scaler 1:16
    ptper = *ptper_not_ps / 16;
    if (ptper <= 32767) {
        *ptckps = 2;
        return EXIT_SUCCESS;
    }
    // pre-scaler 1:64
    ptper = *ptper_not_ps / 64;
    if (ptper <= 32767) {
        *ptckps = 3;
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

int __pwm_prescale_ptper(int *ptckps, long *ptper_not_ps, long *ptper) {
    switch (*ptckps) {
        case 0:
            *ptper = *ptper_not_ps;
            return EXIT_SUCCESS;
        case 1:
            *ptper = *ptper_not_ps / 4;
            return EXIT_SUCCESS;
        case 2:
            *ptper = *ptper_not_ps / 16;
            return EXIT_SUCCESS;
        case 3:
            *ptper = *ptper_not_ps / 64;
            return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

void __pwm_compute_ptper_not_ps(long fpwm, long *ptper_not_ps) {
    *ptper_not_ps = (7372800L / 4L) / (fpwm);
}

void pwm_configuration_free_running(PWM pwm) {
    /**
     * Free running
     * Interrupt when PTMR is reset to 0
     */
    PTCONbits.PTMOD = 0; // Free running

    // Output configuration
    if (pwm.PWM1 == PWM1_ENABLED) {
        PWMCON1bits.PEN1H = 1; // RE1
        PWMCON1bits.PEN1L = 1; // RE0
    }
    if (pwm.PWM2 == PWM2_ENABLED) {
        PWMCON1bits.PEN2H = 1; // RE3
        PWMCON1bits.PEN2L = 1; // RE2
    }
    if (pwm.PWM3 == PWM3_ENABLED) {
        PWMCON1bits.PEN3H = 1; // RE3
        PWMCON1bits.PEN3L = 1; // RE2
    }

    // Period configuration
    // PTPER = Fcy / (ptckps * Fpwm) - 1
    long *ptper_not_ps = malloc(sizeof (long));
    long *ptper = malloc(sizeof (long));
    int *ptckps = malloc(sizeof (int));

    __pwm_compute_ptper_not_ps(pwm.fpwm, ptper_not_ps);
    __pwm_compute_ptckps(ptper_not_ps, ptckps);
    __pwm_prescale_ptper(ptckps, ptper_not_ps, ptper);

    PTPER = *ptper - 1;
    PTCONbits.PTCKPS = *ptckps;

    // Dead time configuration

    long *dta_not_ps = malloc(sizeof (long));
    long *dta = malloc(sizeof (long));
    int *dtaps = malloc(sizeof (int));
    __pwm_compute_dta_not_ps(pwm.dt, dta_not_ps);
    __pwm_compute_dtaps(dta_not_ps, dtaps);
    __pwm_prescale_dta(dtaps, dta_not_ps, dta);

    DTCON1bits.DTA = *dta;
    DTCON1bits.DTAPS = *dtaps; // Prescaler 1:1

    // Enable PWM
    PTEN = 1; // Enable PWM

    // Free memory
    free(dtaps);
    free(dta);
    free(dta_not_ps);
    free(ptckps);
    free(ptper);
    free(ptper_not_ps);
}