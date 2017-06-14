# Author: Aidan Bush
# Date: June, 8, 17
# File: Makefile
# Description: it's a Makefile

SHELL=/bin/bash

CC=gcc
CFLAGS= -Wall -std=c99 -D_POSIX_C_SOURCE=200809L -D_BSD_SOURCE -g

.PHONEY: all clean

all: file_e

file_e: file_e.c file_info.o

file_info.o: file_info.c file_info.h

clean:
	$(RM) *.o file_e
