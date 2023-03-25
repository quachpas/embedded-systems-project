/*
 * File:   ADC.c
 * Author: Pascal Quach <pascal.quach@etu.utc.fr>
 *
 * Created on November 4, 2021, 11:27 AM
 */

#include <stdlib.h>
#include <xc.h>
#include "Timer.h"
#include "ADC.h"

int adc_configuration(ADC adc) {
    // Configure sampling
    if (adc.auto_sampling == AUTO_SAMPLING) {
        ADCON1bits.ASAM = 1;

    } else {
        ADCON1bits.ASAM = 0;
    }

    // Configure conversion
    if (adc.auto_conversion == AUTO_CONVERSION) {
        ADCON1bits.SSRC = 7;
        ADCON3bits.SAMC = 16; // fixed sampling time
    } else {
        ADCON1bits.SSRC = 0;
    }

    // Configure channel
    if (adc.channel == CH0 && adc.scan_mode == SCAN_MODE_ENABLED) {
        ADCON2bits.CHPS = adc.channel;
    } else if (adc.channel >= CH0 && adc.channel <= CH0_CH1_CH2_CH3 && adc.scan_mode == SCAN_MODE_DISABLED) {
        ADCON2bits.CHPS = adc.channel;
    } else {
        return EXIT_FAILURE;
    }

    // Configure T_AD
    ADCON3bits.ADCS = 8; // Tcy * 8

    // Scan mode
    if (adc.scan_mode == SCAN_MODE_ENABLED) {
        ADCON2bits.CSCNA = 1; // scan mode
        ADCSSL = 0; // Reset inputs
    }

    // Configure positive input
    // ADCHSbits.CH0SA; and ADCHSbits.CH123SA;
    if (adc.temperature_meter == TEMPERATURE_METER_UNUSED && adc.potentiometer == POTENTIOMETER_UNUSED) {
        return EXIT_FAILURE;
    }

    // Temperature meter
    if (adc.temperature_meter == TEMPERATURE_METER_USED && adc.scan_mode == SCAN_MODE_DISABLED) {
        ADCHSbits.CH0SA = 2; // AN3 connected to CH0
    } else if (adc.temperature_meter == TEMPERATURE_METER_USED && adc.scan_mode == SCAN_MODE_ENABLED) {
        ADCSSLbits.CSSL3 = 1;
    }

    // Potentiometer
    if (adc.potentiometer == POTENTIOMETER_USED && adc.scan_mode == SCAN_MODE_DISABLED) {
        ADCHSbits.CH123SA = 1; // AN2 connected to CH1
    } else if (adc.potentiometer == POTENTIOMETER_USED && adc.scan_mode == SCAN_MODE_ENABLED) {
        ADCSSLbits.CSSL2 = 1;
    }

    // Configure pins
    ADPCFG = 0xFFFF; // Digital mode for every pin
    if (adc.temperature_meter == TEMPERATURE_METER_USED) {
        ADPCFGbits.PCFG3 = 0; // Analog input AN3
    }
    if (adc.potentiometer == POTENTIOMETER_USED) {
        ADPCFGbits.PCFG2 = 0; // Analog input AN2
    }

    // sequential or simultaneous
    switch (ADCON2bits.CHPS) {
        case 1 ... 3:
            ADCON1bits.SIMSAM = SIMULTANEOUS_SAMPLING;
            break;
        case 0:
            // Not applicable, default value
            ADCON1bits.SIMSAM = SEQUENTIAL_SAMPLING;
            break;
    }

    // Sample per Interrupt
    if (adc.sample_per_interrupt > 0 && adc.sample_per_interrupt <= 16) {
        ADCON2bits.SMPI = adc.sample_per_interrupt - 1;
    }

    return EXIT_SUCCESS;
}

void adc_read_potential(double *adc_value, double *voltage) {
    *voltage = *adc_value / 1024.0 * 5.0;
}

void adc_sample_and_convert(int auto_sampling, int auto_conversion) {
    // Start ADC
    ADCON1bits.ADON = 1;

    if (auto_sampling == 0) {
        // Start manual conversion
        ADCON1bits.SAMP = 1;
    }

    if (auto_conversion == 0) {
        // Wait for sampling
        Timer *t = malloc(sizeof (Timer));
        tmr_get(t, TIMER3, 0);
        tmr_wait_ms(t, 2);
        free(t);

        // Stop sampling and start converting
        ADCON1bits.SAMP = 0;
    }

    // Wait for conversion to be done
    while (ADCON1bits.DONE == 0);

    // Stop ADC
    ADCON1bits.ADON = 0;
}