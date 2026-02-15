// Elijah Greig
// 3128908
// Submitted on: Feb 15 2025
// File: queue.h | Header file for sched application

#ifndef QUEUE_H
#define QUEUE_H
#include "process.h"

void remove_process(struct Process* queue_p, int queue_end);
void push_to_back(struct Process* queue_p, int queue_end);
#endif