# Author: Aidan Bush
# Date: June, 8, 17
# File: Makefile
# Description: it's a Makefile
SHELL=/bin/bash

CC=gcc
CFLAGS=-Wall -Werror -D_POSIX_C_SOURCE=200809L -g

.PHONY: clean

file_info.o: file_info.c file_info.h

open_dirs.o: open_dirs.c open_dirs.h file_info.h

tests: tests.c file_info.o open_dirs.o


clean:
	$(RM) tests *.o
