/*
 * File:   Temp.c
 * Author: quachpas
 *
 * Created on January 7, 2022, 7:18 PM
 */


#include <string.h>
#include "Temp.h"
#include "ADC.h"

void temp_configuration(Temp *t, long size) {
    t->avg_temp = 0;
    t->n_values = 0;
    t->array_size = size;
}

void temp_read(double *adc_value, double *voltage, double *temperature) {
    adc_read_potential(adc_value, voltage);
    *temperature = (*voltage - 0.750) * 100.0 + 25;
}

void temp_add(Temp *t, double *value) {
    if (t->n_values == t->array_size) {
        // Array is full
        // Erase oldest value
        memmove(&t->temp[0], &t->temp[1], sizeof (double) * (t->array_size - 1));

        // Write newest value
        t->temp[t->array_size - 1] = *value;
    } else {
        // Array is either empty or partially empty
        t->temp[t->n_values] = *value;
        t->n_values++;
    }
}

void temp_average(Temp *t) {
    int i;
    int sum = 0;

    for (i = 0; i < t->n_values; i++) {
        sum += t->temp[i];
    }

    t->avg_temp = sum / (double) t->n_values;
};
