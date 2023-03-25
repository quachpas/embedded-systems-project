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
 * File:  Scheduler.h   
 * Author: Pascal Quach
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef SCHEDULER_H
#define	SCHEDULER_H

#define TASKS_NUMBER 10
#define HEARTBEAT 25 // 25 ms

/**
 * Struct: task_t
 * ------------
 * @member number_heartbeats current number of heartbeats elapsed
 * @member execution_period expected number of heartbeats before executing
 * @member task_function pointer to function taking void pointer returning void
 * @member params void pointer
 */
struct task_t {
    int number_heartbeats;
    int execution_period;
    void (*task_function)(void *);
    void* params;
};
typedef struct task_t Task;

/**
 * Function: sch_scheduler
 * ------------------------
 * Schedule tasks and reset number of heartbeats appropriately
 * given an array of Tasks and its size
 * 
 * @param scheduler_tasks array of Task
 * 
 */
void sch_scheduler(Task scheduler_tasks[TASKS_NUMBER]);

#endif	/* SCHEDULER_H */

