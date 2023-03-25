/*
 * File:   Motor.c
 * Author: quachpas
 *
 * Created on January 7, 2022, 7:12 PM
 */


#include <string.h>
#include <stdlib.h>
#include "xc.h"
#include "PWM.h"
#include "Motor.h"

void mtr_configuration(Motor *mtr, Saturations *sats) {
    int refs[N_MOTORS] = {0, 0};
    double dcs[N_MOTORS] = {0.0, 0.0};
    __mtr_set_refs(mtr, refs);
    __mtr_set_dcs(mtr, dcs);
    __mtr_set_sats(mtr, sats);
}

void __mtr_rpm_to_dc(int refs[N_MOTORS], double dcs[N_MOTORS]) {
    int i;
    for (i = 0; i < N_MOTORS; i++) {
        dcs[i] = (1.0 / 24000.0) * refs[i] + 0.5;
    }
}

void __mtr_saturate_rpm(Saturations *sats, int refs[N_MOTORS]) {
    int i;
    for (i = 0; i < N_MOTORS; i++) {
        refs[i] = refs[i] >= sats->rpm_max ? sats->rpm_max : refs[i];
        refs[i] = refs[i] <= sats->rpm_min ? sats->rpm_min : refs[i];
    }
}

void __mtr_set_refs(Motor *mtr, int refs[N_MOTORS]) {
    int i;
    for (i = 0; i < N_MOTORS; i++) {
        mtr->refs[i] = refs[i];
    }
}

void __mtr_set_dcs(Motor *mtr, double dcs[N_MOTORS]) {
    int i;
    for (i = 0; i < N_MOTORS; i++) {
        mtr->dcs[i] = dcs[i];
    }
}

void __mtr_set_sats(Motor *mtr, Saturations *sats) {
    mtr->sats.rpm_max = sats->rpm_max;
    mtr->sats.rpm_min = sats->rpm_min;
}

int __mtr_check_sats(Saturations *sats) {
    int sat_max = SAT_MAX;
    int sat_min = SAT_MIN;
    if (sats->rpm_max >= sat_max ||
            sats->rpm_min <= sat_min ||
            sats->rpm_max < sats->rpm_min ||
            sats->rpm_max <= 0 || sats->rpm_min >= 0) {
        return SATS_OUT_OF_RANGE;
    }

    return EXIT_SUCCESS;
}

void mtr_update_refs(Motor *mtr, int refs[N_MOTORS]) {
    double dcs[N_MOTORS];

    // Saturate and compute duty cycle
    __mtr_saturate_rpm(&mtr->sats, refs);
    __mtr_rpm_to_dc(refs, dcs);

    // Update values in Motor
    __mtr_set_refs(mtr, refs);
    __mtr_set_dcs(mtr, dcs);
}

void mtr_update_sats(Motor *mtr, Saturations *sats) {
    // Update Saturations
    __mtr_set_sats(mtr, sats);

    // Update Motor
    mtr_update_refs(mtr, mtr->refs);
}