/*
 * File:   SPI.c
 * Author: Pascal Quach <pascal.quach@etu.utc.fr>
 *
 * Created on October 26, 2021, 11:28 AM
 */

#include <xc.h>
#include "SPI.h"

void spi_move_cursor(int row, int column) {
    switch (row) {
        case FIRST_ROW:
            spi_put_char(CURSOR_BEGIN_FISRT_ROW + column); // special character
            break;
        case SECOND_ROW:
            spi_put_char(CURSOR_BEGIN_SECOND_ROW + column); // special character
            break;
    }
}

void spi_put_char(char c) {
    while (SPI1STATbits.SPITBF == 1); // Poll until the SPI is ready to write
    SPI1BUF = c;
}

void spi_put_string(char *str) {
    int i = 0;
    for (i = 0; str[i] != '\0'; i++) {
        spi_put_char(str[i]);
    }
}

void spi_clear_first_row() {
    spi_move_cursor(FIRST_ROW, 0);
    int i = 0;
    for (i = 0; i < 16; i++) {
        spi_put_char(' ');
    }
}

void spi_clear_second_row() {
    spi_move_cursor(SECOND_ROW, 0);
    int i = 0;
    for (i = 0; i < 16; i++) {
        spi_put_char(' ');
    }
}

void spi_clear_display() {
    spi_put_char(CLEAR_DISPLAY);
}

void spi_configuration() {
    // Slowing down the transmission the higher the prescaler is
    SPI1CONbits.PPRE = 0b11; // 1:1 
    SPI1CONbits.SPRE = 0b110; // 2:1
    SPI1CONbits.MSTEN = 1; // Master
    SPI1CONbits.MODE16 = 0; // 8 bits
    SPI1STATbits.SPIEN = 1; // enable
}
