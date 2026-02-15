// Elijah Greig
// 3128908
// Submitted on: Feb 15 2025
// File: parse.c | Source file for sched application

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "process.h"


int count_file_lines(char* filename){
    // Count lines in file
    // Arguments: filename
    FILE* file;
    int count = 0;
    int last_char = '\n';
    if (file = fopen(filename, "r")){

        char ch = fgetc(file);
        while (ch != EOF){
            if (last_char == '\n' && isdigit(ch)) {
                count += 1;
            }
            last_char = ch;
            ch = fgetc(file);
        }

        rewind(file);
        fclose(file);
    }
    else { // if file does not exist
        return -1;
    }
    return count;
}

void get_processes(struct Process* p, char* filename) {
    // Parsing processes from file
    // Arguments: array of processes (p), filename

    FILE* file;
    int ignore_line = 0;
    int process_count = 0;
    
    if (file = fopen(filename, "r")){
        char ch = fgetc(file); // read first character

        while (ch != EOF) { // loop to run on every line

            // FIRST: Check if line is commented out
            if (ch == '#') {
                // if so, skip to end of line:
                while ((ch = fgetc(file)) != '\n' && ch != EOF) {} // no action needed
                // move to first character of next line
                ch = fgetc(file);
            }
            else {
                // On line to be read: write line to string
                int line_size = 20; char* line = malloc(line_size); // Starts at size 20. Will be increased if needed
                if (!line) { 
                    printf("Allocation failed\n"); 
                    return;
                }
                int index = 0;

                while (ch != '\n' && ch != -1) {
                    // check if we're full, reallocate to double previous length if so
                    if (index == line_size) {
                        line_size *= 2;
                        line = realloc(line, line_size); 
                        if (!line) { printf("Allocation failed\n"); return;}
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
    fclose(file);
}
