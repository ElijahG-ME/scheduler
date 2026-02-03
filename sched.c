#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
    else {
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
        printf("Usage: ./sched --policy=FCFS|RR [--quantum=N] --in=FILE\n\n");
        return 1;
    }


}