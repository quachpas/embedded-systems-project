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
 * File: Parser.h
 * Author: Enrico Simetti, Pascal Quach <pascal.quach@etu.utc.fr>
 * Comments:
 * Revision history: 
 */

#ifndef PARSER_H
#define	PARSER_H

#include "Messages.h"

#define PARSE_SUCCESS 0
#define PARSE_ERROR -1

/**
 * Struct: parserState_t
 * ---------------------------------------
 * @member state STATE_DOLLAR, STATE_CAT, STATE_TYPE, STATE_PAYLOAD
 * @member index_type
 * @member index_payload
 * @member msg Message
 * 
 */
struct parserState_t {
    int state;
    int index_cat;
    int index_type;
    int index_payload;
    Message msg;
};
typedef struct parserState_t parserState;
struct controller_t;

/**
 * Function: parse_byte
 * -----------------------------------
 * Parse single byte and update parserState
 * 
 * Message format: $PREFIX,data1,data2,...,dataN*
 * with PREFIX an uppercase string 5 characters long.
 * 
 * @param ps
 * @param byte
 * @return NO_MESSAGE or NEW_MESSAGE or PARSE_ERROR 
 * if message format is not respected
 */
int __parse_byte(parserState* ps, char byte);

/**
 * Function: parser_state_initialization
 * ----------------------------------------
 * Initialize parserState
 * 
 * @param ps
 */
void parser_state_configuration(parserState *ps);

/**
 * Function: parse_signed_integer
 * ------------------------------------------------
 * Parse unformatted signed integer +/-... from str[*index]
 * 
 * Stops parsing if "," or \0 is met.
 * 
 * @param str pointer to string
 * @param n pointer to result integer
 * @return PARSE_ERROR if parsed character is not digit
 */
int __parse_signed_integer(const char *str, int *index, int *n);

/**
 * Function: __parse_n_signed_integers
 * ----------------------------------
 * - Parse n signed integers from str 
 * - Write them to arr
 * 
 * @param str
 * @param n
 * @param arr
 * @return 
 */
int __parse_n_signed_integers(const char *str, int n, int arr[n]);

/**
 * Function: __parse_hlref
 * ------------------------------------------------------
 * Parse HLREF.
 * - Update references values
 * 
 * @param ctrl
 * @param ps
 * @return 
 */
int __parse_hlref(struct controller_t *ctrl, parserState *ps);

/**
 * Function: __parse_hlsat
 * ----------------------------------
 * Parse HLSAT.
 * - Check values
 * - Update saturations values
 * - Update references values
 * 
 * @param ctrl
 * @param ps
 * @return 
 */
int __parse_hlsat(struct controller_t *ctrl, parserState *ps);

/**
 * Function: __parse_hlena
 * ----------------------------------------
 * Parse HLENA.
 * For now, always return PARSE_SUCCESS.
 * 
 * @param ctrl
 * @param ps
 * @return 
 * 
 */
int __parse_hlena(struct controller_t *ctrl, parserState *ps);

/**
 * Function: parse_message
 * ---------------------------------
 * If NEW MESSAGE: apply message logic.
 * - Reset timeout timer on HLREF in normal mode.
 * - Change to normal mode on HLREF in timeout.
 * - MCACK on HLSAT
 * - Change to normal mode on HLENA in safe mode.
 *  
 * @param ctrl
 * @param ps
 * @return 
 */
int parse_message(struct controller_t *ctrl, parserState *ps);

#endif	/* PARSER_H */

