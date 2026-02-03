# Elijah Greig
# 3128908
# Submitted on: 
# File: Makefile | Makefile for sched application

GCC= gcc
CFLAGS= -std=c11 -Wall -Wextra -pedantic
VFLAGS = --leak-check=yes

.PHONY: userclean clean valgrind

sched: sched.o
	$(GCC) $(CFLAGS) -o $@ $^ 

%.o: %.c
	$(GCC) -c $<

valgrind: 
	echo "Test username" | valgrind $(VFLAGS) ./sched

clean: 
	rm -f *.o sched