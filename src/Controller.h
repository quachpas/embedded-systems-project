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
 * File:   Controller.h
 * Author: Pascal Quach
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef CONTROLLER_H
#define	CONTROLLER_H

#include "ADC.h"
#include "PWM.h"
#include "Motor.h"
#include "Parser.h"
#include "fbkLCD.h"
#include "Temp.h"

#define MODE_NORMAL 0
#define MODE_TIMEOUT 1
#define MODE_SAFE 2

#define TIMEOUT_TIME 5000

struct controller_t {
    int mode;
    ADC adc;
    PWM pwm;
    Motor mtr;
    parserState ps;
    FbkLCD fbk_lcd;
    Temp t;
};
typedef struct controller_t Controller;

/**
 * Function: ctrl_configuration
 * --------------------------------
 * Configure and initialize Controller.
 * 
 * @param params Controller *ctrl
 */
void ctrl_configuration(Controller *ctrl, PWM pwm, ADC adc, Saturations *sats);

/**
 * Function: __ctrl_change_mode
 * --------------------------------
 * Change the operation mode of 
 * the controller and handle the transition.
 * 
 * @param params Controller *ctrl
 * @param mode MODE_NORMAL, MODE_TIMEOUT, MODE_SAFE
 */
void __ctrl_change_mode(Controller *ctrl, int mode);

/**
 * Function: ctrl_send_fbk
 * --------------------------------
 * Send MCFBK.
 * 
 * @param params Controller *ctrl
 */
void ctrl_send_fbk(void *params);

/**
 * Function: ctrl_pwm
 * --------------------------------
 * Update PDCX.
 * 
 * @param params Controller *ctrl
 * 
 * Notes
 * -----------------------------------------------------
 * The PWW is in free running and complementary output mode.
 * When PDCX = PTMR / 2, the PWM output is set low.
 * If we set PDCX to = dc * 2 * PTPER then:
 * - dc = 0.5, then PDCX = PTPER, and we have (1/2) 50% DC
 * - dc = 0.75, then PDCX = 1.5 PTPER, (1.5/2) 75% DC
 * etc.
 */
void ctrl_pwm(void *params);

/**
 * Function: ctrl_poll
 * --------------------------------
 * Poll for and handle UART messages.
 * 
 * @param params Controller *ctrl
 */
void ctrl_poll(void *params);

/**
 * Function: ctrl_blink_D4
 * --------------------------------
 * Blink LED D4 (Timeout indicator)
 * 
 * @param params Controller *ctrl
 */
void ctrl_blink_D4(void *params);

/**
 * Function: ctrl_blink_D3
 * --------------------------------
 * Blink LED D3 (All time)
 * 
 * @param params Controller *ctrl
 */
void ctrl_blink_D3(void *params);

/**
 * Function: ctrl_check_S5
 * --------------------------------
 * Send Button S5, which changes the 
 * operation mode of the controller
 * to safe mode.
 * 
 * @param params Controller *ctrl
 */
void ctrl_check_S5(void *params);

/**
 * Function: ctrl_check_S6
 * --------------------------------
 * Send Button S6, which changes the 
 * page of the feedback display.
 * 
 * @param params Controller *ctrl
 */
void ctrl_check_S6(void *params);

/**
 * Function: ctrl_write_lcd
 * ---------------------------------------
 * Write feedback on LCD depending on current page.
 *
 * (First screen)
 * <--------16--------->
 * ┌───────────────────┐
 * │ST: x; T: y        │
 * │R: n1; n2          │
 * └───────────────────┘
 * where:
 *  - x is H/T/C (Halt/Timeout/Controlled)
 *  - y is the temperature (%.1f)
 *  - n1 and n2 are applied RPM (signed)
 * 
 * (Second screen)
 * ┌───────────────────┐
 * │SA: x; y           │
 * │R: PDC1; PDC2      │
 * └───────────────────┘
 * where:
 *  - x and y are the saturation values
 *  - PDC1 and PDC2 are the values of the PWM duty cycle registers
 *
 * @param params Controller *ctrl
 */
void ctrl_write_lcd(void *params);

/**
 * Function: ctrl_read_temp
 * ---------------------------------------
 * Read temperature voltage from ADCBUF0 and
 * convert to °C. 
 * Add to temperature history and re-compute average.
 * 
 * @param params Controller *ctrl
 */
void ctrl_read_temp(void *params);

/**
 * Function: ctrl_send_tem
 * --------------------------------
 * Send MCTEM
 * 
 * @param params Controller *ctrl
 */
void ctrl_send_tem(void *params);
#endif	/* CONTROLLER_H */

