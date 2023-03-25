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
 * File: ADC.h  
 * Author: Pascal Quach <pascal.quach@etu.utc.fr>
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef ADC_H
#define	ADC_H

#define AUTO_SAMPLING 1
#define MANUAL_SAMPLING 0
#define AUTO_CONVERSION 1
#define MANUAL_CONVERSION 0
#define CH0 0
#define CH0_CH1 1
#define CH0_CH1_CH2_CH3 3
#define TEMPERATURE_METER_USED 1 
#define TEMPERATURE_METER_UNUSED 0
#define POTENTIOMETER_USED 1
#define POTENTIOMETER_UNUSED 0
#define SCAN_MODE_ENABLED 1
#define SCAN_MODE_DISABLED 0
#define SIMULTANEOUS_SAMPLING 1 
#define SEQUENTIAL_SAMPLING 0

/**
 * Struct: adc_t
 * ------------------------------------
 * @member auto_sampling
 * @member auto_conversion
 * @member channel
 * @member temperature_meter
 * @member potentiometer
 * @member scan_mode
 * @member sample_per_interrupt
 */
struct adc_t {
    int auto_sampling;
    int auto_conversion;
    int channel;
    int temperature_meter;
    int potentiometer;
    int scan_mode;
    int sample_per_interrupt;
};
typedef struct adc_t ADC;


/**
 * Function: adc_configuration
 * -----------------------------
 * Configure ADC for automatic/manual sampling/conversion
 * of potentiometer and temperature meter on board.
 * 
 * Unless scan mode is enabled:
 * - Temperature meter (AN3) is connected to CH0
 * - Potentiometer (AN2) is connected to CH1
 * When scan mode is enabled, AN2 and AN3 are selected
 * for input scan.
 * 
 * When both sensors are used and scan mode is not enabled,
 * simultaneous sampling is used.
 * Otherwise, sequential sampling is used.
 * 
 * The A/D conversion period is approximately 2,443 ns (ADCS)
 * The A/D sampling time is configured to be 16 * T_AD (SAMC)
 * 
 * Sample per interrupt should be an integer between 1 and 16 included.
 * 
 * @param adc
 * @return EXIT_FAILURE or EXIT_SUCCESS
 * 
 * Notes
 * ----------------------------
 * ADCS = 2 * T_AD / Tcy - 1
 */
int adc_configuration(ADC adc);

/**
 * Function: adc_read_potential
 * ---------------------------------------------------
 * Returns a voltage value between 0 and 5.0 V.
 * 
 * @param adc_value
 * @param voltage
 * 
 * Notes
 * --------------------
 * We use 10-bit ADC, thus the resolution is ADC/1024.
 */
void adc_read_potential(double *adc_value, double *voltage);

/**
 * Function: adc_sample_and_convert
 * -----------------------------------------
 * Wrapping function for sampling and conversion regardless of
 * automatic or manual mode. 
 * 
 * @param auto_sampling
 * @param auto_conversion
 */
void adc_sample_and_convert(int auto_sampling, int auto_conversion);

#endif	/* ADC_H */

