// Elijah Greig
// 3128908
// Submitted on: Feb 15 2025
// File: sched.c | Source file for sched application

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parse.h"
#include "queue.h"
#include "process.h"


void compute_stats(struct Process* queue, int process_count, int context_switches){
    // Function to compute and print out process & scheduler stats
    // Arguments: array of processes (queue), process_count, context_switches
    float total_TAT = 0;
    float total_RESP = 0;
    
    for (int i = 0; i < process_count; i++){
        if (queue[i].PID != -1) {
            int id = queue[i].PID;
            int first_run = queue[i].first_run;
            int completion = queue[i].completion;
            int turnaround = completion - queue[i].arrival;
            int response = first_run - queue[i].arrival;

            total_TAT += turnaround;
            total_RESP += response;
            
            printf("P%d: first run=%d completion=%d TAT=%d RESP=%d\n", id, first_run, completion, turnaround, response);
        }
        
        
    }
    printf("System: ctx_switches=%d, avgTAT=%.3f, avgRESP=%.3f\n", context_switches, (total_TAT/process_count), (total_RESP/process_count));

}

void fcfs(struct Process* p, int process_count){
    // FCFS scheduler:
    // Arguments: pointer to array of processes (p), process count

    int process_using = -1; // currently running process PID (-1 for none)
    int processes_completed = 0;

    struct Process queue[process_count];
    struct Process* queue_p = queue;
    int in_queue = 0; // index pointing to most recent queue entry
    int queue_pos = 0; // index pointing to where cpu is currently in the queue

    // trackers
    int context_switches = process_count - 1; // in FCFS, context switches is always 1 less than total number of processes
    int t_size = 30;
    char* time_elapsed = malloc(t_size); // Strings to hold scheduler gantt chart info
    char* run_elapsed = malloc(t_size); // Begins at size 30, will be increased if needed
    if (!time_elapsed || !run_elapsed) { printf("Allocation failed\n"); return;}
    time_elapsed[0] = '\0';
    run_elapsed[0] = '\0';
    strcat(time_elapsed, "time:");
    strcat(run_elapsed, "run :");

    // fill queue with empty processes (all values -1) for easy pointer reading
    for (int i = 0; i < process_count; i++) { queue[i].PID = -1; queue[i].arrival = -1;}

    int time = 0;

    while (processes_completed != process_count){
        // Time begins here
        
        // First: Check all processes to see if any arrive at current time. If so, add all to queue
        for (int i = 0; i < process_count; i++){

            if (p[i].arrival == time) { queue_p[in_queue++] = p[i];}
            
        }

        // Next: if CPU is free, assign process & begin usage 
        if (process_using == -1) {
            if (queue[queue_pos].PID != -1) {
                process_using = queue[queue_pos].PID; // pos of queue is current job (for easy pointer management)
                queue[queue_pos].first_run = time;
                queue[queue_pos].time_spent++;
            } 

        }
        else { // otherwise, continue current job
            queue[queue_pos].time_spent++;
        }

        // Record Info

        // First: Check if re-allocation needed
        if ((t_size - strlen(time_elapsed) <= 10) || t_size - strlen(run_elapsed) <= 10  ){ // triggers when characters approach end of allocation (within the last 10 characters)
            t_size *= 2; // doubles size and continues
            time_elapsed = realloc(time_elapsed, t_size); 
            run_elapsed = realloc(run_elapsed, t_size);
            
            if (!time_elapsed || !run_elapsed) { printf("Allocation failed\n"); return;}
        }

        // Second: append to string this cycle's info
        sprintf(time_elapsed + strlen(time_elapsed), " %d", time);
        queue[queue_pos].PID == -1 ? sprintf(run_elapsed + strlen(run_elapsed), " %s", "-") : sprintf(run_elapsed + strlen(run_elapsed), " %d", queue[queue_pos].PID);
        
        


        // check if job is done
        if (queue[queue_pos].PID != -1 && (queue[queue_pos].time_spent == queue[queue_pos].cpu_time)){
            process_using = -1;
            queue[queue_pos].completion = time+1; // completion time recorded to process data
            queue_pos++; processes_completed++;
        }

        time++;

    }

    // Print stats and free allocations
    printf("%s\n", time_elapsed);
    printf("%s\n", run_elapsed);
    compute_stats(queue, process_count, context_switches);
    free(time_elapsed);
    free(run_elapsed);

}

void RR(struct Process* p, int quantum, int process_count){
    // Round Robin scheduler:
    // Arguments: pointer to array of processes (p), quantum, process count

    int process_using = -1; // currently running process PID (-1 for none)
    int processes_completed = 0;
    int order = 0; // used to organize metric output syntax (allows processes to be printed in order of lowest first_run first)

    struct Process queue[process_count];
    struct Process* queue_p = queue;
    int queue_end = 0; // index pointing to 1 past most recent queue entry
    int process_time = 0; // amount of time process has ran for (measured against quantum)
    int in_queue = 0; // amount of processes currently in queue;
    struct Process finished_queue[process_count]; // array to hold processes that have finished running (for data output)

    // trackers
    int context_switches = 0;
    int t_size = 30;
    char* time_elapsed = malloc(t_size); // Begins at size 30, will be increased if needed
    char* run_elapsed = malloc(t_size); 
    if (!time_elapsed || !run_elapsed) { printf("Allocation failed\n"); return;}
    time_elapsed[0] = '\0';
    run_elapsed[0] = '\0';
    strcat(time_elapsed, "time:");
    strcat(run_elapsed, "run :");

    // fill queue with empty processes (all values -1) for easy pointer reading
    for (int i = 0; i < process_count; i++) { queue[i].PID = -1; queue[i].arrival = -1;}

    int time = 0;

    while (processes_completed != process_count){
        // Time begins here
        
        // First: Check all processes to see if any arrive at current time. If so, add all to queue
        for (int i = 0; i < process_count; i++){

            if (p[i].arrival == time) { 
                queue_p[queue_end++] = p[i]; 
                in_queue++; 
                queue[i].first_run = -1; // First run is assigned -1 until it runs, at which point it changes to current time
                queue[i].order = order++; // tracking order of arrival for output syntax, not related to scheduler statistics
            }
            
        }

        // Next: Assign process (if empty) & begin usage
        if (process_using == -1) {
            if (queue[0].PID != -1) {
                process_using = queue[0].PID; 
                if (queue[0].first_run == -1) { queue[0].first_run = time; } // assigns "first run" statistic if not already assigned
                queue[0].time_spent++; process_time++; // Trackers
            } 

        }
        else { // otherwise, continue current job
            queue[0].time_spent++; process_time++;
            process_using = queue[0].PID; 
            if (queue[0].first_run == -1) { queue[0].first_run = time; } // assigns "first run" statistic if not already assigned
            
        }

        
        // Record Info ----

        // First: Check if re-allocation needed
        if ((t_size - strlen(time_elapsed) <= 10) || t_size - strlen(run_elapsed) <= 10  ){ // triggers when characters approach end of allocation (within the last 10 characters)
            t_size *= 2; // doubles size and continues
            time_elapsed = realloc(time_elapsed, t_size); 
            run_elapsed = realloc(run_elapsed, t_size);
            
            if (!time_elapsed || !run_elapsed) { printf("Allocation failed\n"); return;}
        }

        // Second: append to string this cycle's info
        sprintf(time_elapsed + strlen(time_elapsed), " %d", time);
        queue[0].PID == -1 ? sprintf(run_elapsed + strlen(run_elapsed), " %s", "-") : sprintf(run_elapsed + strlen(run_elapsed), " %d", queue[0].PID);
        
        //-----------------
        

        // check if job is done
        if (queue[0].PID != -1 && (queue[0].time_spent == queue[0].cpu_time)){
            process_using = -1;
            queue[0].completion = time+1;
            finished_queue[queue[0].order] = queue[0];
            processes_completed++; 
            remove_process(queue_p, queue_end--);
            process_time = 0; in_queue--;
            if (processes_completed != process_count) { context_switches++; }

        }


        // Next: Check if quantum is up
        if (process_time == quantum) {
            if (in_queue != 1) { push_to_back(queue_p, queue_end-1); context_switches++; } // No need to push everything back if the queue is one item long
            process_time = 0; 
        }
    
        time++;

    }

    printf("%s\n", time_elapsed);
    printf("%s\n", run_elapsed);
    compute_stats(finished_queue, process_count, context_switches);
    free(time_elapsed);
    free(run_elapsed);

}


int main(int argc, char *argv[]){

    // Check if number of args is valid
    if (argc > 4 || argc < 3){
        printf("Usage: ./sched --policy=FCFS|RR [--quantum=N] --in=FILE\n");
        return 1;
    }

    // Policy Codes:
    // 1 for FCFS
    // 2 for RR
    int policy = 0;

    // evaluating the first arg

    if (strcmp(argv[1], "--policy=FCFS") == 0){ 
        policy = 1;
    }
    else if (strcmp(argv[1], "--policy=RR") == 0){
        policy = 2;
    }
    else if (policy == 0){
        printf("Usage: ./sched --policy=FCFS|RR [--quantum=N] --in=FILE\n");
        return 1;
    }

    int fileindex = 2; // index of "--in=" argument in argv
    int quantum = 0;

    // checking quantum value
    if (policy == 2){  
        fileindex = 3; // increasing file argument index to account for quantum argument

        if (strncmp(argv[2], "--quantum=", 10) == 0){ // comparing first 10 characters of the arg string for correct usage
            quantum = atoi(argv[2] + 10);
            
        }
        else {
            printf("Usage: ./sched --policy=FCFS|RR [--quantum=N] --in=FILE\n");
            return 1;
        }
    
        if (quantum < 0){
            printf("Usage: ./sched --policy=FCFS|RR [--quantum=N] --in=FILE\n");
            return 1;
        }
    }

    // checking file

    char* filename;
    if (strncmp(argv[fileindex], "--in=", 5) == 0){ // comparing first 5 characters of the arg string for correct usage
        filename = malloc(strlen(argv[fileindex] + 5) + 1); // moves pointer to count length of string past equals sign
        if (filename == NULL) {
                printf("Memory allocation failed");
                return 1;
            } 
        strcpy(filename, argv[fileindex] + 5);


    }
    else {
        printf("Usage: ./sched --policy=FCFS|RR [--quantum=N] --in=FILE\n");
        return 1;
    }
    
    int process_count = count_file_lines(filename); // count number of processes in the file
    if (process_count == -1) { // -1 is returned only if the file does not exist
        printf("File not found\n"); 
        return 1;
    }


    // array of all processes, unsorted, in order of file
    struct Process processes[process_count];
    struct Process *p = processes;

    get_processes(p, filename); 

    switch(policy) {
        case 1:
            fcfs(p, process_count);
        break;

        case 2:
            RR(p, quantum, process_count);
        break;
    }

    free(filename);
    return 0;

}
