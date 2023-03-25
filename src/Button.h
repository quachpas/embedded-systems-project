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
 * File: Button.h
 * Author: Pascal Quach <pascal.quach@etu.utc.fr>
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef BUTTON_H
#define	BUTTON_H

#include <xc.h> // include processor files - each processor file is guarded.  

// Buttons
#define BUTTON_PRESSED 1
#define BUTTON_NORMAL 0
#define S_BUTTON_NORMAL 1
#define S_BUTTON_PRESSED 0
#define BUTTON_S5 5
#define BUTTON_S6 6
#define BUTTON_S5_TRIS TRISEbits.TRISE8
#define BUTTON_S5_PORT PORTEbits.RE8
#define BUTTON_S5_LAT LATEbits.LATE8
#define BUTTON_S6_TRIS TRISDbits.TRISD0
#define BUTTON_S6_PORT PORTDbits.RD0
#define BUTTON_S6_LAT LATDbits.LATD0

/**
 * Function: but_check_button_pressed
 * -------------------------------------------------
 * Check whether button was pressed and previously not pressed
 * 
 * @param button BUTTON_S5 OR BUTTON_S6
 */
int but_check_button_pressed(int button);
#endif	/* BUTTON_H */

