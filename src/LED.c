/*
 * File:   LED.c
 * Author: Pascal Quach <pascal.quach@etu.utc.fr>
 *
 * Created on September 27, 2021, 4:31 PM
 */

#include "LED.h"

void led_D4_blink() { // 2 Hz when timeout
    LED_D4_LAT = !LED_D4_LAT;
}

void led_D3_blink() { // 1 Hz
    LED_D3_LAT = !LED_D3_LAT;
}