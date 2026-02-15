// Elijah Greig
// 3128908
// Submitted on: Feb 15 2025
// File: parse.h | Header file for sched application

#ifndef PARSE_H
#define PARSE_H
#include "process.h"

int count_file_lines(char* filename);
void get_processes(struct Process* p, char* filename);
#endif