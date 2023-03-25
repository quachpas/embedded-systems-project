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
 * File:   fbkLCD.h
 * Author: Pascal Quach
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef FBK_LCD_H
#define	FBK_LCD_H

#include "SPI.h"

#define NUMBER_PAGES 2

#define SEMI_COLON "; "
#define COLON ": "

#define ST "ST"
#define SA "SA"
#define T "T"
#define R "R"

/**
 * Struct: fkb_lcd_t
 * ----------------------------------------------
 * @member page_index
 * @member number_pages
 */
struct fbk_lcd_t {
    int page_index;
    int number_pages;
};
typedef struct fbk_lcd_t FbkLCD;

/**
 * Function: fbk_lcd_next_screen
 * --------------------------------------------------
 * Next screen
 * @param fbk_lcd
 */
void fbk_lcd_next_screen(FbkLCD *fbk_lcd);

/**
 * Function: fbk_lcd_configuration
 * ----------------------------------------
 * Initialize and configure FbkLCD
 * 
 * @param fbk_lcd
 * @param pages
 */
void fbk_lcd_configuration(FbkLCD *fbk_lcd, int pages);

/**
 * Function: __fbk_lcd_write_screen
 * --------------------------------------------------
 * Write LCD rows.
 * 
 * @param str1
 * @param str2
 */
void __fbk_lcd_write_screen(char str1[LCD_ROW_LENGTH + 1], char str2[LCD_ROW_LENGTH + 1]);

/**
 * Function: fbk_lcd_p1r1
 * ---------------------------------------------
 * Serialize fbkLCD, page 1, row 1 to str
 * 
 * @param state
 * @param temp
 * @param str
 * @return 
 */
char* fbk_lcd_p1r1(int *state, double *temp, char str[LCD_ROW_LENGTH + 1]);

/**
 * Function: fbk_lcd_p1r2
 * ---------------------------------------------
 * Serialize fbkLCD, page 1, row 2 to str
 * 
 * @param state
 * @param temp
 * @param str
 * @return 
 */
char* fbk_lcd_p1r2(int *n1, int *n2, char str[LCD_ROW_LENGTH + 1]);

/**
 * Function: fbk_lcd_p2r1
 * ---------------------------------------------
 * Serialize fbkLCD, page 2, row 1 to str
 * 
 * @param state
 * @param temp
 * @param str
 * @return 
 */
char* fbk_lcd_p2r1(int *rpm_max, int *rpm_min, char str[LCD_ROW_LENGTH + 1]);

/**
 * Function: fbk_lcd_p2r2
 * ---------------------------------------------
 * Serialize fbkLCD, page 2, row 2 to str
 * 
 * @param state
 * @param temp
 * @param str
 * @return 
 */
char* fbk_lcd_p2r2(double *dc1, double *dc2, char str[LCD_ROW_LENGTH + 1]);
#endif	/* FBK_LCD_H */

