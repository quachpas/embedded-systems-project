/*
 * File:   main.c
 * Author: Pascal Quach <pascal.quach@etu.utc.fr>
 *
 * Created on October 12, 2021, 3:08 PM
 */
// FOSC
#pragma config FPR = XT                 // Primary Oscillator Mode (XT)
#pragma config FOS = PRI                // Oscillator Source (Primary Oscillator)
#pragma config FCKSMEN = CSW_FSCM_OFF   // Clock Switching and Monitor (Sw Disabled, Mon Disabled)

// FWDT
#pragma config FWPSB = WDTPSB_16        // WDT Prescaler B (1:16)
#pragma config FWPSA = WDTPSA_512       // WDT Prescaler A (1:512)
#pragma config WDT = WDT_OFF            // Watchdog Timer (Disabled)

// FBORPOR
#pragma config FPWRT = PWRT_64          // POR Timer Value (64ms)
#pragma config BODENV = BORV20          // Brown Out Voltage (Reserved)
#pragma config BOREN = PBOR_ON          // PBOR Enable (Enabled)
#pragma config LPOL = PWMxL_ACT_HI      // Low-side PWM Output Polarity (Active High)
#pragma config HPOL = PWMxH_ACT_HI      // High-side PWM Output Polarity (Active High)
#pragma config PWMPIN = RST_IOPIN       // PWM Output Pin Reset (Control with PORT/TRIS regs)
#pragma config MCLRE = MCLR_EN          // Master Clear Enable (Enabled)

// FGS
#pragma config GWRP = GWRP_OFF          // General Code Segment Write Protect (Disabled)
#pragma config GCP = CODE_PROT_OFF      // General Segment Code Protection (Disabled)

// FICD
#pragma config ICS = ICS_PGD            // Comm Channel Select (Use PGC/EMUC and PGD/EMUD)

/** 
 * ------------------------------------
 * MOTOR CONFIGURATION (PWM)
 * ------------------------------------
 * Theoretical range: +/- 12k RPMs
 * Desired range: +/- 9k RPM
 * Fpwm: 1 kHz
 * Duty cycle centered on 0 RPM:
 *  - 0%, -12k RPM
 *  - 50%, 0 RPM
 *  - 100%, 12k RPM
 * Dead time: At least 3 µs
 */

/**
 * ----------------------------------------
 * CONTROL SYSTEM
 * ----------------------------------------
 * The control system receives desired reference values (HLREF)
 * for the motors rotation speed in RPMs through a serial
 * interface. The values are saturated to the desired range
 * for the motor and propeller (see motor configuration).
 * 
 * The control system communicates with a control PC to
 * report status information.
 *
 * ---------------------
 * Operations modes
 * ---------------------
 * (Timeout) 
 * If HLREF is not received for more than 5 seconds,
 * then:
 *  - Motor velocity set to 0 RPMs
 *  - LED D4 blinks at 1 Hz to signal timeout
 *  - If HLREF is received, the control system quits
 * timeout
 * 
 * (Normal)
 * The control system:
 *  - supports receiving HLREF at least at 10 Hz
 *  - supports updating the PWM at least at 10 Hz
 *  - does not ACK HLREF
 *  
 * 
 * (Safe)
 * Safe mode is entered if button S5 is pressed
 *  - Motors stop immediately
 *  - HLREF are ignored
 *  - If HLENA is received:
 *      a. Send ACK to PC
 *      b. exit safe mode
 *      c. Motors move after new HLREF is received
 *  
 * (ALL)
 *  - LED D3 blinks at 1 Hz to signal the main loop
 *  - sends back MCFBK at 5 kHz
 */

/**
 * ----------------------------------------------------
 * LCD (SPI)
 * ----------------------------------------------------
 * The LCD displays either the first screen or the 
 * second one as described below.
 * The button S6 changes the display.
 * 
 * 
 * (First screen)
 * <--------16--------->
 * ┌───────────────────┐
 * │ST: x; T: y        │
 * │R: n1; n2          │
 * └───────────────────┘
 * where:
 *  - x is H/T/C (Halt/Timeout/Controlled)
 *  - y is the temperature (%.1f)
 *  - n1 and n2 are applied RPM (signed)
 * 
 * (Second screen)
 * ┌───────────────────┐
 * │SA: x; y           │
 * │R: PDC1; PDC2      │
 * └───────────────────┘
 * where:
 *  - x and y are the saturation values
 *  - PDC1 and PDC2 are the values of the PWM duty cycle registers
 */

/**
 * -----------------------------------------------------
 * SENSORS (ADC)
 * -----------------------------------------------------
 * (Temperature)
 *  - Temperature is acquired at 10 Hz.
 *  - The average value is sent to the PC at 1 Hz (MCTEM)
 */

/**
 * --------------------------------------------------------
 * MESSAGES (UART2)
 * --------------------------------------------------------
 * Messages should never be lost, even with full
 * use of the bandwidth.
 * 
 * --------------------------------------------------------
 * SENT MESSAGES (UART2)
 * --------------------------------------------------------
 * (MCFBK): motor
 * ______________________
 * $MCFBK,n1,n2,state*
 * ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾
 * 
 * State:
 *  - 0, normal mode
 *  - 1, timeout mode
 *  - 2, safe mode
 * 
 * (MCTEM): temperature
 * ________________
 * $MCTEM,temp*
 * ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾
 * Temperature is %.1f
 * 
 * (MCACK): acknowledge message
 * ______________________
 * $MCACK,msg_type,value*
 * ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾
 * msg_type is the command sent, i.e. SAT or ENA
 * 
 * value:
 *  - 0, NACK
 *  - 1, ACK
 * 
 * ---------------------------------------------------------
 * RECEIVED MESSAGES (UART2)
 * ---------------------------------------------------------
 * (HLREF): desired references values
 * ________________
 * $HLREF,n1,n2*
 * ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾
 * (HLSAT): RPM saturation values
 * ________________
 * $HLSAT,min,max*
 * ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾
 * Preliminary checks:
 *  - Check +/- 9k
 *  - Check min < max
 *  - min <= 0, max >= 0
 *  
 * Fail: send NACK
 * Pass: saturation values are updated and send ACK
 * 
 * (HLENA): exit safe mode
 * ________________
 * $HLENA*
 * ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾
 * Send ACK to PC if in safe mode
 * 
 */

#include <string.h>
#include <stdlib.h>
#include "Timer.h"
#include "LED.h"
#include "Button.h"
#include "PWM.h"
#include "ADC.h"
#include "Interrupt.h"
#include "SPI.h"
#include "UART.h"
#include "circularBuffer.h"
#include "Controller.h"
#include "Motor.h"
#include "Temp.h"
#include "Scheduler.h"
#include "Messages.h"
#include "fbkLCD.h"
#include "extern.h"

Controller *ctrl;
volatile circularBuffer *cb_rxreg;
volatile circularBuffer *cb_txreg;

int main(void) {
    // Timer configuration
    Timer *tmr_main_loop = malloc(sizeof (Timer));
    Timer *tmr_timeout = malloc(sizeof (Timer));
    tmr_get(tmr_main_loop, TIMER_MAIN_LOOP, 0);
    tmr_get(tmr_timeout, TIMER_TIMEOUT, 1);
    tmr_setup_period(tmr_main_loop, HEARTBEAT);
    tmr_setup_period(tmr_timeout, TIMEOUT_TIME);

    // LED Configuration
    LED_D3_TRIS = 0;
    LED_D4_TRIS = 0;

    // Button configurations
    BUTTON_S5_TRIS = 1;
    BUTTON_S6_TRIS = 1;

    // SPI configuration
    spi_configuration();

    // PWM Configuration
    PWM pwm = {MTR_FPWM, MTR_DT, PWM1_ENABLED, PWM2_ENABLED, PWM3_DISABLED};


    // ADC Configuration
    ADC adc = {AUTO_SAMPLING, AUTO_CONVERSION, CH0,
        TEMPERATURE_METER_USED, POTENTIOMETER_UNUSED, SCAN_MODE_DISABLED, 1};

    // UART2 Configuration
    uart2_configuration(UART2_BAUD_RATE); // 8, N, 1

    // Circular buffer
    cb_rxreg = malloc(sizeof (circularBuffer) + CB_SIZE * sizeof (char));
    cb_configuration(cb_rxreg, CB_SIZE);
    cb_txreg = malloc(sizeof (circularBuffer) + CB_SIZE * sizeof (char));
    cb_configuration(cb_txreg, CB_SIZE);

    // Controller 
    ctrl = malloc(sizeof (Controller) + TEMP_HISTORY * sizeof (double));
    Saturations sats;
    sats.rpm_max = SAT_MAX;
    sats.rpm_min = SAT_MIN;
    ctrl_configuration(ctrl, pwm, adc, &sats);

    // Initialize scheduler
    // Define TASKS_NUMBER n Scheduler.h
    // Define HEARTBEAT in Scheduler.h
    Task scheduler_tasks[TASKS_NUMBER];

    /*                              **Scheduler table**
     * --------------------------------------------------------------------------------
     * |       task       | period [ms]  | period [hb=25ms] | max execution time [ms] |
     * | :--------------: | :----------: | :--------------: | :---------------------: |
     * |  poll messages   | 25(**<100**) |        1         |           6             |
     * |   control pwm    | 25(**<100**) |        1         |          0.6            |
     * | check button s5  |      25      |        1         |          0.03           |
     * | check button s6  |      25      |        1         |          0.03           |
     * |    write lcd     |     100      |        4         |           4             |
     * | read temperature |   **100**    |        4         |           4+            |
     * |    send mcfbk    |   **200**    |        8         |           3             |
     * |    send mctem    |   **1000**   |        40        |          5              |
     * |     blink d4     |   **1000**   |        40        |          0.03           |
     * |     blink d3     |   **1000**   |        40        |          0.05           |
     * --------------------------------------------------------------------------------
     */

    // Schedule tasks
    // Poll Messages
    scheduler_tasks[0].number_heartbeats = 0;
    scheduler_tasks[0].execution_period = 1;
    scheduler_tasks[0].task_function = ctrl_poll;
    scheduler_tasks[0].params = (void*) ctrl;

    // Control PWM
    scheduler_tasks[1].number_heartbeats = 0;
    scheduler_tasks[1].execution_period = 1;
    scheduler_tasks[1].task_function = ctrl_pwm;
    scheduler_tasks[1].params = (void*) ctrl;

    // Check Button S5
    scheduler_tasks[2].number_heartbeats = 0;
    scheduler_tasks[2].execution_period = 1;
    scheduler_tasks[2].task_function = ctrl_check_S5;
    scheduler_tasks[2].params = (void*) ctrl;

    // Check Button S6
    scheduler_tasks[3].number_heartbeats = 0;
    scheduler_tasks[3].execution_period = 1;
    scheduler_tasks[3].task_function = ctrl_check_S6;
    scheduler_tasks[3].params = (void*) ctrl;

    // Write LCD
    scheduler_tasks[4].number_heartbeats = 0;
    scheduler_tasks[4].execution_period = 4;
    scheduler_tasks[4].task_function = ctrl_write_lcd;
    scheduler_tasks[4].params = (void*) ctrl;

    // Read temperature
    scheduler_tasks[5].number_heartbeats = 0;
    scheduler_tasks[5].execution_period = 4;
    scheduler_tasks[5].task_function = ctrl_read_temp;
    scheduler_tasks[5].params = (void*) ctrl;

    // Send MCFBK
    scheduler_tasks[6].number_heartbeats = 0;
    scheduler_tasks[6].execution_period = 8;
    scheduler_tasks[6].task_function = ctrl_send_fbk;
    scheduler_tasks[6].params = (void*) ctrl;

    // Send MCTEM
    scheduler_tasks[7].number_heartbeats = 0;
    scheduler_tasks[7].execution_period = 40;
    scheduler_tasks[7].task_function = ctrl_send_tem;
    scheduler_tasks[7].params = (void*) ctrl;

    // Blink D4
    scheduler_tasks[8].number_heartbeats = 0;
    scheduler_tasks[8].execution_period = 40;
    scheduler_tasks[8].task_function = ctrl_blink_D4;
    scheduler_tasks[8].params = (void*) ctrl;

    // Blink D3
    scheduler_tasks[9].number_heartbeats = 0;
    scheduler_tasks[9].execution_period = 40;
    scheduler_tasks[9].task_function = ctrl_blink_D3;
    scheduler_tasks[9].params = (void*) ctrl;


    // Main loop
    tmr_start_stop(tmr_timeout, 1);
    while (1) {
        tmr_start_stop(tmr_main_loop, 1);

        sch_scheduler(scheduler_tasks); // 20 ms

        tmr_wait_period(tmr_main_loop);
    }

    // Free memory
    free(ctrl);
    free((void*) cb_rxreg);
    free((void*) cb_txreg);
    free(tmr_timeout);
    free(tmr_main_loop);

    return EXIT_SUCCESS;
};