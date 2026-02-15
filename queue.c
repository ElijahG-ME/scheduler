// Elijah Greig
// 3128908
// Submitted on: Feb 15 2025
// File: queue.c | Source file for sched application

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "process.h"


void remove_process(struct Process* queue_p, int queue_end){
    // Remove process from front of queue
    // Arguments: array of processes (queue_p), position of last item in queue (queue_end)
    for (int i = 0; i < queue_end; i++){
        queue_p[i] = queue_p[i+1];
    }
    queue_p[queue_end-1].PID = -1;

}

void push_to_back(struct Process* queue_p, int queue_end){
    // Remove process from front of queue and place it at the end
    // Arguments: array of processes (queue_p), position of last item in queue (queue_end)
    struct Process front = queue_p[0];
    for (int i = 0; i < queue_end; i++){
        queue_p[i] = queue_p[i+1];
    }
    queue_p[queue_end] = front;
}
