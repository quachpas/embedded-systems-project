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
 * File:   Serialize.h
 * Author: Pascal Quach
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef SERIALIZE_H
#define	SERIALIZE_H

#include "Messages.h"
#include "SPI.h"

#define SERIALIZE_ERROR "-1"
#define S_MODE_NORMAL "C"
#define S_MODE_TIMEOUT "T"
#define S_MODE_SAFE "H"

/**
 * Function: __serialize_msg
 * ------------------------------
 * Serialize MSG $[CAT][TYPE],[PAYLOAD]*
 * 
 * @param msg
 * @param str
 */
void __serialize_msg(Message *msg, char str[MSG_LENGTH]);


/**
 * Function: __serialize_unsigned_double
 * -------------------------------------------------
 * %.1f
 * 
 * @param d
 * @param str
 * @return pointer to str
 */
char* __serialize_unsigned_double(double *d, char *str);

/**
 * Function: __serialize_signed_integer
 * ---------------------------------
 * +%d
 * 
 * @param n
 * @param str
 * @return pointer to str
 */
char* __serialize_signed_integer(int *n, char *str);

/**
 * Function: __serialize_unsigned_integer
 * ---------------------------------------
 * n > 0
 * %d
 * 
 * @param n
 * @param str
 * @return 
 */
char* __serialize_unsigned_integer(int *n, char *str);

/**
 * Function: __serialize_controller_state
 * ----------------------------------------------------------
 * H/T/C
 * 
 * @param state
 * @param str
 * @return 
 */
char* __serialize_controller_state(int *state, char *str);

/**
 * Function: __serialize_payload_fbk
 * --------------------------------------------------
 * n1,n2,state
 * 
 * @param n1
 * @param n2
 * @param state
 * @param str
 */
void __serialize_payload_fbk(int *n1, int *n2, int *state, char str[MSG_PAYLOAD_LENGTH]);

/**
 * Function: __serialize_payload_tem
 * ----------------------------------------------------------------
 * temp
 * @param temp
 * @param str
 */
void __serialize_payload_tem(double *temp, char str[MSG_PAYLOAD_LENGTH]);

/**
 * Function: __serialize_payload_ack
 * ----------------------------------------------------------------
 * msg_type,H/T/C
 * 
 * @param temp
 * @param str
 */
void __serialize_payload_ack(char msg_type[4], int *value, char str[MSG_PAYLOAD_LENGTH]);

/**
 * Function: __serialize_fbk_lcd_p1r1
 * ----------------------------------------------------------------
 * ST: H/T/C; T: temp
 * @param temp
 * @param str
 */
void __serialize_fbk_lcd_p1r1(int *state, double *temp, char str[LCD_ROW_LENGTH + 1]);

/**
 * Function: __serialize_fbk_lcd_p1r2
 * ----------------------------------------------------------------
 * R: n1; n2
 * 
 * @param temp
 * @param str
 */
void __serialize_fbk_lcd_p1r2(int *n1, int *n2, char str[LCD_ROW_LENGTH + 1]);

/**
 * Function: __serialize_fbk_lcd_p2r1
 * ----------------------------------------------------------------
 * SA: rpm_min; rpm_max
 * @param temp
 * @param str
 */
void __serialize_fbk_lcd_p2r1(int *rpm_max, int *rpm_min, char str[LCD_ROW_LENGTH + 1]);

/**
 * Function: __serialize_fbk_lcd_p2r2
 * ----------------------------------------------------------------
 * R: dc1; dc2
 * @param temp
 * @param str
 */
void __serialize_fbk_lcd_p2r2(double *dc1, double *dc2, char str[LCD_ROW_LENGTH + 1]);

#endif	/* SERIALIZE_H */

