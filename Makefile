# Author: Aidan Bush
# Date: June, 8, 17
# File: Makefile
# Description: it's a Makefile

SHELL=/bin/bash

CC=gcc
CFLAGS= -Wall -std=c99 -D_POSIX_C_SOURCE=200809L -D_DEFAULT_SOURCE -g
LDLIBS= -lncurses

.PHONEY: all clean

all: file_view

file_view: file_view.o file_info.o open_directory.o file_search.o file_ncurses.o

file_view.o: file_view.c file_info.h open_directory.h

file_info.o: file_info.c file_info.h

open_directory.o: open_directory.c open_directory.h file_info.h

file_search.o: file_search.c file_search.h open_directory.h file_info.h

file_ncurses.o: file_ncurses.c file_ncurses.h

clean:
	$(RM) *.o file_view
