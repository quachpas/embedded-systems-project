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
 * File: Motor.h    
 * Author: Pascal Quach
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef MOTOR_H
#define	MOTOR_H

#define MTR_FPWM 1000L // Hz
#define MTR_DT 3000L // ns

#define N_MOTORS 2

#define SAT_MAX 9000;
#define SAT_MIN -9000;

#define SATS_OUT_OF_RANGE -1

/** 
 * Struct: sat_t
 * ------------------------------------------
 * @member rpm_max
 * @member rpm_min
 */
struct sat_t {
    int rpm_max;
    int rpm_min;
};
typedef struct sat_t Saturations;

/**
 * Struct: motor_t
 * ----------------------------------------
 * @member refs[N_MOTORS]
 * @member sats 
 * @member dcs[N_MOTORS]
 */
struct motor_t {
    int refs[N_MOTORS];
    Saturations sats;
    double dcs[N_MOTORS];
};
typedef struct motor_t Motor;

/**
 * Function: mtr_configuration
 * ---------------------------------------------------------------------
 * Initialize Motor with given saturations.
 * 
 * @param mtr
 * @param sats
 */
void mtr_configuration(Motor *mtr, Saturations *sats);

/**
 * Function: __mtr_rpm_to_dc
 * -------------------------------
 * Convert linearly given reference to DC value according to:
 * - 0%, -12000
 * - 50%, 0
 * - 100%, 12000.
 * 
 * @param refs
 * @param dcs
 */
void __mtr_rpm_to_dc(int refs[N_MOTORS], double dcs[N_MOTORS]);

/**
 * Function: __mtr_saturate_rpm
 * -------------------------------
 * Saturate references with given saturations
 * 
 * @param sats
 * @param refs
 */
void __mtr_saturate_rpm(Saturations *sats, int refs[N_MOTORS]);

/**
 * Function: __mtr_set_refs
 * -------------------------------------------
 * Set references
 * 
 * @param mtr
 * @param refs
 */
void __mtr_set_refs(Motor *mtr, int refs[N_MOTORS]);

/**
 * Function: __mtr_set_dcs
 * -------------------------------------------
 * Set duty cycles
 * 
 * @param mtr
 * @param refs
 */
void __mtr_set_dcs(Motor *mtr, double dcs[N_MOTORS]);

/**
 * Function: __mtr_set_sats
 * -------------------------------------------
 * Set saturations
 * 
 * @param mtr
 * @param refs
 */
void __mtr_set_sats(Motor *mtr, Saturations *sats);

/**
 * Function: __mtr_check_sats
 * ----------------------------------------------
 * Check:
 * - Saturations in [-9000, 9000] interval (propeller safety)
 * - max > min
 * - max >= 0, min <= 0
 * @param sats
 * @return 
 */
int __mtr_check_sats(Saturations *sats);

/**
 * Function: mtr_update_refs
 * -----------------------------------------
 * Update motor with new references:
 * - Saturate references
 * - Update refs & dcs
 * @param mtr
 * @param refs
 */
void mtr_update_refs(Motor *mtr, int refs[N_MOTORS]);

/**
 * Function: mtr_update_sats
 * ---------------------------------------
 * Update motor with new saturations:
 * - Update saturations
 * - Update motor references
 */
void mtr_update_sats(Motor *mtr, Saturations *sats);

#endif	/* MOTOR_H */

