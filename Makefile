# Author: Aidan Bush
# Date: June, 8, 17
# File: Makefile
# Description: it's a Makefile

SHELL=/bin/bash

CC=gcc
CFLAGS= -Wall -std=c99 -D_POSIX_C_SOURCE=200809L -D_BSD_SOURCE -g

.PHONEY: all clean

all: file_view

file_view: file_view.o file_info.o open_directory.o

file_view.o: file_view.c file_info.h open_directory.h

file_info.o: file_info.c file_info.h

open_directory.o: open_directory.c open_directory.h file_info.h

clean:
	$(RM) *.o file_view
