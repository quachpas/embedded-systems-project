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
 * File: SPI.h
 * Author: Pascal Quach <pascal.quach@etu.utc.fr>
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef SPI_H
#define	SPI_H

#define LCD_ROW_LENGTH 16
#define FIRST_ROW 0
#define SECOND_ROW 1
#define CURSOR_DECREMENT 0x4
#define CURSOR_INCREMENT 0x6
#define CURSOR_BEGIN_FISRT_ROW 0x80
#define CURSOR_BEGIN_SECOND_ROW 0xC0
#define DISPLAY_ON_CURSOR_ON 0xE
#define CLEAR_DISPLAY 0x1


/**
 * Function: spi_move_cursor
 * -------------------------
 * Move the cursor to given position
 * 
 * @param row 0 or 1
 * @param column from 0 to 15
 */
void spi_move_cursor(int row, int column);

/**
 * Function: spi_put_char
 * ---------------------------
 * Put character in transmit buffer
 * @param c pointer to char c
 */
void spi_put_char(char c);

/**
 * Function: spi_put_string
 * -------------------------
 * Put whole string in transmit buffer
 * @param str char*
 */
void spi_put_string(char *str);

/**
 * Function: spi_clear_first_row
 * ------------------------------------------
 * Clear the first row
 */
void spi_clear_first_row();

/**
 * Function: spi_clear_second_row
 * ------------------------------------------
 * Clear the second row
 */
void spi_clear_second_row();

/**
 * Function: spi_clear_display
 * --------------------------------
 * Clear the display
 */
void spi_clear_display();

/**
 * Function: spi_configuration
 * -----------------------------
 * Enable SPI and configure in Master Mode, byte comm. 
 * and 2:1 prescale.
 * 
 * Primary Prescale (Master Mode) 
 * PPRE -> 1:1 
 * 
 * Secondary Prescale (Master Mode)
 * SPRE -> 2:1
 * 
 * Master Mode
 * MSTEN -> Master Mode
 * 
 * Word/Byte Comm
 * MODE16 -> byte
 * 
 * SPI Enable
 * SPIEN -> Enabled
 * 
 */
void spi_configuration();

#endif	/* SPI_H */