# Author: Aidan Bush
# Date: June, 8, 17
# File: Makefile
# Description: it's a Makefile
SHELL=/bin/bash

CC=gcc
CFLAGS=-Wall -Werror -D_POSIX_C_SOURCE=200809L -g

.PHONY: all clean

all: mydirstat

mydirstat: mydirstat.o search.o file_info.o open_dirs.o

mydistat.o: mydirstat.c

file_info.o: file_info.c file_info.h

open_dirs.o: open_dirs.c open_dirs.h file_info.h

search.o: search.c search.h open_dirs.h file_info.h

#testing
tests: tests.c file_info.o open_dirs.o search.o

clean:
	$(RM) mydirstat tests *.o
