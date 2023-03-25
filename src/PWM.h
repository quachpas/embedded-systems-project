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
 * File: PWM.h
 * Author: Pascal Quach <pascal.quach@etu.utc.fr>
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef PWM_H
#define	PWM_H

#include <xc.h>

/**
 * General Notes
 * PDC1 register controls PWM1H/PWM1L outputs 
 * PDC2 register controls PWM2H/PWM2L outputs 
 * PDC3 register controls PWM3H/PWM3L outputs
 */
#define PWM1_ENABLED 1 
#define PWM1_DISABLED 0
#define PWM2_ENABLED 1 
#define PWM2_DISABLED 0
#define PWM3_ENABLED 1 
#define PWM3_DISABLED 0
#define PTEN PTCONbits.PTEN

/**
 * Struct: pwm_t
 * -----------------------------
 * @member fpwm
 * @member dt
 * @member PWM1
 * @member PWM2
 * @member PWM3
 */
struct pwm_t {
    long fpwm;
    long dt;
    int PWM1;
    int PWM2;
    int PWM3;
};
typedef struct pwm_t PWM;

/**
 * Function: pwm_configuration_free_running
 * ------------------------------------------
 * Configure PWM in free running mode
 * according to desired PWM frequency (Hz)
 * and desired deadtime (ns)
 * 
 * @param pwm pwm_t
 * 
 */
void pwm_configuration_free_running(PWM pwm);

/**
 * Function: __pwm_compute_dtaps
 * --------------------------------------------------------------
 * Compute scale factor for DTA
 * 
 * @param dta_not_ps
 * @param dtaps
 * @return  EXIT_SUCCESS or EXIT_FAILURE
 * 
 * Notes
 * -------------------------
 * DTA is 6 bits
 */
int __pwm_compute_dtaps(long *dta_not_ps, int *dtaps);

/**
 * Function: __pwm_prescale_dta
 * -------------------------------------------
 * Scale absolute DTA value
 * 
 * DTA = Dead time (ns) / (dtaps * Tcy)
 
 * @param dtaps
 * @param dta_not_ps
 * @param dta
 * @return EXIT_SUCCESS or EXIT_FAILURE
 * 
 */
long __pwm_prescale_dta(int *dtaps, long *dta_not_ps, long *dta);

/**
 * Function: __pwm_compute_dta_not_ps
 * ----------------------------------------------
 * Compute absolute DTA
 * 
 * DTA_abs = Dead time (ns) / Tcy
 * @param dead_time
 * @param dta_not_ps
 * 
 * Notes
 * --------------
 * Tcy = 543 ns
 */
void __pwm_compute_dta_not_ps(long dead_time, long *dta_not_ps);

/**
 * Function: __pwm_compute_ptckps
 * -----------------------------------------
 * Compute scale factor for PTPER
 * 
 * @param ticks_not_ps
 * @param ptckps
 * @return 
 * 
 * Notes
 * -------------------------------
 * PTPER is 15 bits
 */
int __pwm_compute_ptckps(long *ticks_not_ps, int *ptckps);

/**
 * Function: __pwm_prescale_ptper
 * -----------------------------------------
 * Scale absolute PTPER value 
 * 
 * @param ptckps
 * @param ticks_not_ps
 * @param ticks
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int __pwm_prescale_ptper(int *ptckps, long *ticks_not_ps, long *ticks);

/**
 * Function: __pwm_compute_ptper_not_ps
 * -------------------------------------
 * Compute absolute PTPER for free running mode.
 * 
 * @param fpwm
 * @param ticks_not_ps
 * 
 * Notes
 * --------------------------
 * PTPER = Fcy / (PTCKPS * Fpwm) - 1
 */
void __pwm_compute_ptper_not_ps(long fpwm, long *ticks_not_ps);

#endif /* PWM_H */