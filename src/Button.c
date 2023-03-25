/*
 * File:   Button.c
 * Author: Pascal Quach
 *
 * Created on November 8, 2021, 3:15 PM
 */



#include "stdlib.h"
#include "Button.h"
#include "Interrupt.h"

int but_check_button_pressed(int button) {
    static int prevButton = S_BUTTON_NORMAL;
    int currButton;
    switch (button) {
        case BUTTON_S5:
            currButton = BUTTON_S5_PORT;
            break;
        case BUTTON_S6:
            currButton = BUTTON_S6_PORT;
            break;
    }
    if (currButton == S_BUTTON_PRESSED && prevButton == S_BUTTON_NORMAL) {
        prevButton = currButton;
        return BUTTON_PRESSED;
    } else {
        prevButton = currButton;
        return BUTTON_NORMAL;
    }
}