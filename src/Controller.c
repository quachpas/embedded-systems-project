/*
 * File:   Controller.c
 * Author: quachpas
 *
 * Created on January 7, 2022, 7:17 PM
 */
// lib
#include "Button.h"
#include "ADC.h"
#include "SPI.h"
#include "Temp.h"
#include "PWM.h"
#include "LED.h"
#include "Messages.h"
#include "Controller.h"
#include "Parser.h"
#include <stdlib.h>

void ctrl_configuration(Controller *ctrl, PWM pwm, ADC adc, Saturations *sats) {
    ctrl->mode = MODE_NORMAL;
    ctrl->pwm = pwm;
    ctrl->adc = adc;
    adc_configuration(ctrl->adc);
    pwm_configuration_free_running(ctrl->pwm); // 1 kHz, 3 µs
    mtr_configuration(&ctrl->mtr, sats);
    parser_state_configuration(&ctrl->ps);
    fbk_lcd_configuration(&ctrl->fbk_lcd, NUMBER_PAGES);
    temp_configuration(&ctrl->t, TEMP_HISTORY);
}

void ctrl_send_fbk(void *params) {
    Controller *ctrl = (Controller *) params;
    Message msg;

    msg_fbk(&ctrl->mtr.refs[0], &ctrl->mtr.refs[1], &ctrl->mode, &msg);
    msg_send(&msg);
}

void ctrl_pwm(void *params) {
    Controller *ctrl = (Controller *) params;
    if (ctrl->mode == MODE_NORMAL) {
        if (ctrl->pwm.PWM1) {
            PDC1 = ctrl->mtr.dcs[0] * 2.0 * PTPER;
        }
        if (ctrl->pwm.PWM2) {
            PDC2 = ctrl->mtr.dcs[1] * 2.0 * PTPER;
        }
        if (ctrl->pwm.PWM3) {
            PDC3 = ctrl->mtr.dcs[2] * 2.0 * PTPER;
        }
    }
}

void ctrl_poll(void *params) {
    Controller *ctrl = (Controller *) params;
    msg_poll(ctrl, &ctrl->ps);
}

void __ctrl_change_mode(Controller *ctrl, int mode) {
    if (mode == MODE_NORMAL || mode == MODE_TIMEOUT || mode == MODE_SAFE) {
        ctrl->mode = mode;
    }

    // Transition
    if (ctrl->mode == MODE_NORMAL) {
        PTEN = 1;
    } else if (ctrl->mode == MODE_TIMEOUT) {
        int refs[2] = {0, 0};
        mtr_update_refs(&ctrl->mtr, refs);
    } else if (ctrl->mode == MODE_SAFE) {
        PTEN = 0;
    }
}

void ctrl_blink_D4(void *params) {
    Controller *ctrl = (Controller *) params;
    if (ctrl->mode == MODE_TIMEOUT) {
        led_D4_blink();
    }
}

void ctrl_blink_D3(void *params) {
    led_D3_blink();
}

void ctrl_send_tem(void *params) {
    Controller *ctrl = (Controller *) params;
    Message msg;

    msg_tem(&ctrl->t.avg_temp, &msg);
    msg_send(&msg);
}

void ctrl_check_S5(void *params) {
    Controller *ctrl = (Controller *) params;
    if (but_check_button_pressed(BUTTON_S5)) {
        __ctrl_change_mode(ctrl, MODE_SAFE);
    }
}

void ctrl_check_S6(void *params) {
    Controller *ctrl = (Controller *) params;
    if (but_check_button_pressed(BUTTON_S6)) {
        fbk_lcd_next_screen(&ctrl->fbk_lcd);
    }
}

void ctrl_write_lcd(void *params) {
    Controller *ctrl = (Controller *) params;
    char str_r1[LCD_ROW_LENGTH + 1];
    char str_r2[LCD_ROW_LENGTH + 1];
    spi_clear_display();
    if (ctrl->fbk_lcd.page_index == 1) {
        fbk_lcd_p1r1(&ctrl->mode, &ctrl->t.avg_temp, str_r1);
        fbk_lcd_p1r2(&ctrl->mtr.refs[0], &ctrl->mtr.refs[1], str_r2);
    } else if (ctrl->fbk_lcd.page_index == 2) {
        fbk_lcd_p2r1(&ctrl->mtr.sats.rpm_max, &ctrl->mtr.sats.rpm_min, str_r1);
        fbk_lcd_p2r2(&ctrl->mtr.dcs[0], &ctrl->mtr.dcs[1], str_r2);
    }
}

void ctrl_read_temp(void *params) {
    Controller *ctrl = (Controller *) params;
    adc_sample_and_convert(ctrl->adc.auto_sampling, ctrl->adc.auto_conversion);

    double adc_value_temp = ADCBUF0;
    double voltageTemp;
    double temperature;
    temp_read(&adc_value_temp, &voltageTemp, &temperature);
    temp_add(&ctrl->t, &temperature);
    temp_average(&ctrl->t);
}