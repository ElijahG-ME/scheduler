Elijah Greig
3128908
Submitted on: Feb 15 2025
File: README.md | Info for sched application

# Sched

Sched will simulate a scheduler's logic and return output and metrics with a given input.
Logic for First Come First Serve (FCFS) and Round Robin (RR) scheduling has been created and CPU cycles will be simulated using loops.
Processes are read from a file with valid formatting and will be used in the scheduling simulation.

## How RR and FCFS differ
Using example W1.txt, in a FCFS simulation, the average turnaround time is 8 seconds, and the average response time is 4 seconds (one cycle a second).
In a RR simulation with a quantum of 1, the average turnaround time is still 8 seconds, however the average response time is down to 1 second, which creates a faster feel for a system user, not having to wait as long to receive feedback. This comes with the highest number of context switches, however, creating additional overhead.
In a RR simulation with a quantum of 4, the average turnaround time increases to 8.3 seconds, and the average response time is 3 seconds, meaning that it takes a process longer to be ran, and longer to be completed, making it laggy and unresponsive. FCFS has a lower turnaround time and a lower number of context switches.

# How to run

To compile: run ``make``.

**Usage: ./sched --policy=FCFS|RR [--quantum=N] --in=FILE**
    - quantum is only necessary for RR policy

# Program status

Program runs with no known issues or deficiencies when the file input is in a valid format. Program may hang or crash if the file is not formatted properly.

File formatting guide: 
    - Comment lines must begin with #.
    - One line represents a single process.
    - The format must be: `[Process ID] [Arrival Time] [Time to Completion]` with no brackets and only a single space between each number. All 3 variables must be numeric ONLY.
        - Example:
            # Comment line - disregarded
            # Comment line - disregarded
            0 0 3
            1 1 1
            2 5 7



## Integrity Statement
I certify that this submission represents entirely my own work.


