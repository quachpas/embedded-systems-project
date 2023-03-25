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
 * File:   Temp.h
 * Author: Pascal Quach
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef TEMP_H
#define	TEMP_H

#define TEMP_HISTORY 10

/**
 * Struct: temp_t
 * --------------------------
 * @member array_size
 * @member n_values
 * @member avg_temp
 * @member temp[TEMP_HISTORY]
 */
struct temp_t {
    int array_size;
    int n_values;
    double avg_temp;
    double temp[TEMP_HISTORY];
};
typedef struct temp_t Temp;

/**
 * Function: temp_configuration
 * ---------------------------------------------
 * Initialize temp_t with array size size
 * 
 * @param t
 * @param size
 */
void temp_configuration(Temp *t, long size);

/**
 * Function: adc_read_temperature
 * -----------------------------------
 * Given the value from the ADC, and returns two results for
 * the voltage and temperature w.r.t. 
 * - 0.750 V <-> 25°C
 * - 1 V <-> 100°C
 * - Temperature is linear from -40°C to 125°C
 * - We use a 10-bit ADC
 * 
 * @param voltage
 * @param temperature
 * @param adc_value
 * 
 * Notes
 * -----------------------------------------------
 * Temperature function is the linear interpolation between 
 * (0 V, 25°C) and (V, Temperature) knowing that the slope is 
 * exactly 0.010 V/°C
 * 
 */
void temp_read(double *adc_value, double *voltage, double *temperature);

/**
 * Function: temp_add
 * ------------------------------------------
 * Add temperature to array.
 * Discard oldest value.
 * 
 * @param t
 * @param value
 */
void temp_add(Temp *t, double *value);

/**
 * Function: temp_average
 * ---------------------------
 * Compute average temperature of Temp t
 * 
 * @param t
 */
void temp_average(Temp *t);
#endif	/* XC_HEADER_TEMPLATE_H */

