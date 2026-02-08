#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Process {
    int PID;
    int arrival;
    int cpu_time;
    int time_spent;

    // metrics
    int first_run;
    int completion;
};



int count_file(char* filename){
    // Count lines in file
    FILE* file;
    int ignore_line = 0;
    int count = 0;
    int last_char = '\n';
    if (file = fopen(filename, "r")){

        int ch = fgetc(file);
        while (ch != EOF){
            if (last_char == '\n' && ch == '#') {
                count -= 1;
            }
            if (ch == '\n') {
                count += 1;
            }

            last_char = ch;
            ch = fgetc(file);
        }

        rewind(file);
    }
    
    return count;
}

void get_processes(struct Process* p, char* filename) {
    FILE* file;
    int ignore_line = 0;
    int process_count = 0;
    if (file = fopen(filename, "r")){
        char ch = fgetc(file); // read first character

        while (ch != EOF) { // loop to run on every line

            // FIRST: Check if line is commented out
            if (ch == '#') {
                // if so, skip to end of line:
                while (ch != '\n') { ch = fgetc(file); }
                // move to first character of next line
                ch = fgetc(file);
            }
            else {
                // On line to be read: write line to string
                int line_size = 20; char* line = malloc(line_size); // Starts at size 20. Will be increased if needed
                int index = 0;
                while (ch != '\n') {
                    // check if we're full, reallocate to double if so
                    if (index == line_size) {
                        line_size *= 2;
                        line = realloc(line, line_size); // to do later: ensure realloc was successful
                    }
                    line[index] = ch;
                    index += 1;
                    ch = fgetc(file);

                }
                // Now at end of line
                line[index] = '\0'; // add null terminator

                // tokenize line at spaces and take values into process
                p[process_count].PID = atoi(strtok(line, " "));
                p[process_count].arrival = atoi(strtok(NULL, " "));
                p[process_count].cpu_time = atoi(strtok(NULL, " "));
                p[process_count].time_spent = 0;

                // free line for reuse and increment process count, then move to first character of next line
                free(line);
                process_count++;
                ch = fgetc(file);
                
            }
        }
    }
}

void fcfs(struct Process* p, int process_count){
    // FCFS scheduler:
    int process_using = -1; // currently running process PID (-1 for none)
    int processes_completed = 0;

    struct Process queue[process_count];
    struct Process* queue_p = queue;
    int in_queue = 0; // index pointing to most recent queue entry
    int queue_pos = 0; // index pointing to where cpu is currently in the queue

    // fill queue with empty processes (all values -1) for easy pointer reading
    for (int i = 0; i < process_count; i++) { queue[i].PID = -1; queue[i].arrival = -1;}

    int time = 0;

    while (processes_completed != process_count){
        // Time begins here

        /*---------
        printf("Current Queue: ");
        for (int i = 0; i < process_count; i++){
            printf("%d | ", queue[i].PID);

        }
        printf("\n");
        */
        
        // First: Check all processes to see if any arrive at current time. If so, add all to queue
        for (int i = 0; i < process_count; i++){

            if (p[i].arrival == time) { queue_p[in_queue] = p[i]; in_queue++;}
            
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

        // Record
        printf("time: %d pid: %d\n", time, queue[queue_pos].PID);

        // check if job is done
        if (queue[queue_pos].time_spent == queue[queue_pos].cpu_time){
            process_using = -1;
            queue[queue_pos].completion = time;
            queue_pos++;
            processes_completed++;
        }

        time++;

    }


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
            printf("    Usage: ./sched --policy=FCFS|RR [--quantum=N] --in=FILE\n");
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

    int process_count = count_file(filename);

    // array of all processes, unsorted, in order of file
    struct Process processes[process_count];
    struct Process *p = processes;

    get_processes(p, filename);
    fcfs(p, process_count);

    return 0;

}
