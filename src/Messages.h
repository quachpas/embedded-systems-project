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
 * File:   Messages.h
 * Author: Pascal Quach
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef MESSAGES_H
#define	MESSAGES_H

#define DOLLAR "$"
#define COMMA ","
#define STAR "*"

#define STATE_DOLLAR  1 // we discard everything until a dollar is found
#define STATE_CAT   2 // Read MSG_TYPE_LENGTH characters
#define STATE_TYPE    3 // we are reading the type of msg until a comma is found
#define STATE_PAYLOAD 4 // we read the payload until an asterix is found

#define NEW_MESSAGE 1 // new message received and parsed completely
#define NO_MESSAGE 0 // no new messages

#define MSG_CAT_LENGTH 2
#define MSG_TYPE_LENGTH 3
#define MSG_PAYLOAD_LENGTH 20
#define MSG_LENGTH 30

#define HL "HL"
#define REF "REF"
#define SAT "SAT"
#define ENA "ENA"

#define MC "MC"
#define FBK "FBK"
#define TEM "TEM"
#define ACK "ACK"

#define _NACK 0
#define _ACK 1

#define UART2_BAUD_RATE 9600L

/**
 * Struct: msg_t
 * ------------------------------
 * @member cat[MSG_CAT_LENGTH + 1]
 * @member type[MSG_TYPE_LENGTH + 1]
 * @member payload[MSG_PAYLOAD_LENGTH + 1]
 */
struct msg_t {
    char cat[MSG_CAT_LENGTH + 1]; // category is 2 chars + \0
    char type[MSG_TYPE_LENGTH + 1]; // type is 3 chars + \0
    char payload[MSG_PAYLOAD_LENGTH + 1]; // assume payload cannot be longer than MSG_PAYLOAD_LENGTH chars
};
typedef struct msg_t Message;
struct controller_t;
struct parserState_t;

/**
 * Function: msg_poll
 * ----------------------------------------
 * Poll for messages using parser
 * 
 * @param ctrl
 * @param ps
 */
void msg_poll(struct controller_t *ctrl, struct parserState_t *ps);

/**
 * Function: msg_fbk
 * --------------------------------
 * Create Message containing MCFBK
 * @param n1
 * @param n2
 * @param state
 * @param msg
 */
void msg_fbk(int *n1, int *n2, int *state, Message *msg);

/**
 * Function: msg_tem
 * ----------------------------
 * Create Message containing MCTEM
 * 
 * @param temp
 * @param msg
 */
void msg_tem(double *temp, Message *msg);

/**
 * Function: msg_ack
 * ----------------------------------
 * Create Message containing MCACK
 * 
 * @param msg_type
 * @param value
 * @param msg
 */
void msg_ack(char msg_type[4], int value, Message *msg);

/**
 * Function: msg_send
 * ------------------------------------------------
 * Send a message using UART2
 * 
 * @param msg
 */
void msg_send(Message *msg);

/**
 * Function: msg_send_ack
 * --------------------------------
 * Send MCACK
 * @param msg_type
 * @param value
 */
void msg_send_ack(char msg_type[MSG_TYPE_LENGTH + 1], int value);
#endif	/* MESSAGES_H*/

