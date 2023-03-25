/*
 * File:   Scheduler.c
 * Author: Pascal Quach
 *
 * Created on November 18, 2021, 11:27 AM
 */

#include "Scheduler.h"

void sch_scheduler(Task scheduler_tasks[TASKS_NUMBER]) {
    int i;
    for (i = 0; i < TASKS_NUMBER; i++) {
        scheduler_tasks[i].number_heartbeats++;

        if (scheduler_tasks[i].number_heartbeats >= scheduler_tasks[i].execution_period) {
            scheduler_tasks[i].task_function(scheduler_tasks[i].params);
            scheduler_tasks[i].number_heartbeats = 0;
        }
    }
}