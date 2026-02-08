#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Process {
    int PID;
    int arrival;
    int cpu_time;
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
    int last_char = '\n';
    if (file = fopen(filename, "r")){

        int ch = fgetc(file);
        while (ch != EOF){
            int val_current_size = 4;
            char* val = malloc(val_current_size); // begins with size 4, will realloc if necessary

            if (last_char == '\n' && ch == '#') {
                while ((ch = fgetc(file)) != '\n') {} // skips line if it begins with #      
                last_char = ch;
                ch = fgetc(file);
            }

            if (last_char == '\n' && isdigit(ch)) { // algorithm truly starts here
                printf("first character: %c\n", ch);
                // new line of file begins here

                for (int id = 0; id < 3; id++){ // id refers to what variable to assign: pid, arrival, cpu time
                    int pos = 0; // position of "string" for variable recording
                    

                    while (ch != ' ' && ch != '\n') {
                        
                        if (pos == val_current_size - 1) {  // re-allocating if more size is needed
                            char* tmp = realloc(val, (val_current_size * 2) + 1);
                            if (!tmp) { free(val); exit(1);}
                            val = tmp; 
                            val_current_size *= 2;

                        }

                        val[pos] = ch;
                        pos += 1;
                        //printf("pos: %d ch: %c val: %s\n", pos, ch, val);
                        ch = fgetc(file);
                

                    }
                    val[pos] = '\0';
                    
                    

                    switch(id) {
                        case 0:
                            int pid = atoi(val);
                            p[process_count].PID = pid;
                            printf("pid %d\n", atoi(val));
                            break;
                        case 1:
                            int arr = atoi(val);
                            p[process_count].arrival = arr;
                            printf("arr %d\n", atoi(val));
                            break;
                        case 2:
                            int cpu = atoi(val);
                            p[process_count].cpu_time = cpu;
                            printf("cpu %d\n", atoi(val));
                            break;
                    }
                    
                    val[0] = '\0';
                    if (ch == ' ') {
                        ch = fgetc(file); 
                    }
                }
                process_count += 1;
                
                
            }

            last_char = ch;
            ch = fgetc(file);
        }

        rewind(file);
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
    printf("%d\n", process_count );

    // array of all processes, unsorted, in order of file
    struct Process processes[process_count];
    struct Process *p = processes;

    get_processes(p, filename);

      //----------
    for (int i = 0; i < 3; i++){
        printf("\npid: %d ", processes[i].PID);
        
        printf("arr: %d ", processes[i].arrival);
        
        printf("cpu: %d", processes[i].cpu_time);
    }
    //----------

    return 0;

}
