# Elijah Greig
# 3128908
# Submitted on: Feb 15 2025
# File: Makefile | Makefile for sched application

GCC= gcc
CFLAGS= -std=c11 -Wall -Wextra -pedantic
VFLAGS = --leak-check=yes

.PHONY: userclean clean valgrind

all: sched.o parse.o queue.o
	$(GCC) $(CFLAGS) -o sched $^ 

%.o: %.c
	$(GCC) -c $<

valgrind: 
	valgrind $(VFLAGS) ./sched --policy=FCFS --in=W1.txt

clean: 
	rm -f *.o sched