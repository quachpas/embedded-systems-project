/*
 * File:   fbkLCD.c
 * Author: quachpas
 *
 * Created on January 7, 2022, 7:15 PM
 */

// lib
#include "Button.h"
#include "SPI.h"
#include "fbkLCD.h"
#include "Serialize.h"

void fbk_lcd_next_screen(FbkLCD *fbk_lcd) {
    fbk_lcd->page_index = (fbk_lcd->page_index + 1) % fbk_lcd->number_pages;
}

void fbk_lcd_configuration(FbkLCD *fbk_lcd, int pages) {
    fbk_lcd->number_pages = pages;
    fbk_lcd->page_index = 1;
}

void __fbk_lcd_write_screen(char str1[LCD_ROW_LENGTH + 1], char str2[LCD_ROW_LENGTH + 1]) {
    spi_move_cursor(FIRST_ROW, 0);
    spi_put_string(str1);
    spi_move_cursor(SECOND_ROW, 0);
    spi_put_string(str2);
}

char* fbk_lcd_p1r1(int *state, double *temp, char str[LCD_ROW_LENGTH + 1]) {
    __serialize_fbk_lcd_p1r1(state, temp, str);
    return str;
}

char* fbk_lcd_p1r2(int *n1, int *n2, char str[LCD_ROW_LENGTH + 1]) {
    __serialize_fbk_lcd_p1r2(n1, n2, str);
    return str;
}

char* fbk_lcd_p2r1(int *rpm_max, int *rpm_min, char str[LCD_ROW_LENGTH + 1]) {
    __serialize_fbk_lcd_p2r1(rpm_max, rpm_min, str);
    return str;
}

char* fbk_lcd_p2r2(double *dc1, double *dc2, char str[LCD_ROW_LENGTH + 1]) {
    __serialize_fbk_lcd_p2r2(dc1, dc2, str);
    return str;
}