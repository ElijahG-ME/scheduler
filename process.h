// Elijah Greig
// 3128908
// Submitted on: Feb 15 2025
// File: process.h | Header file for sched application
#ifndef PROCESS_H
#define PROCESS_H

struct Process {
    int PID;
    int arrival;
    int cpu_time;
    int time_spent;

    // metrics
    int first_run;
    int completion;
    int order; // used only for RR output print syntax, not real metric
};
#endif