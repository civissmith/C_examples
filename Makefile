################################################################################
# @Title: Makefile
#
# @Author: Phil Smith
#
# @Date: Thu, 23-Apr-15 10:26PM
#
# @Project: Terminal I/O
#
# @Purpose: Build the different components in the test directory.
#
#
################################################################################
CC := gcc

# Create a list of all of the files to be built
targets := ansi_funcs.c
targets += arrow_keys.c
targets += getpass.c
targets += winsize.c

# Convert foo.c to foo.out
executables := $(subst .c,.out,$(targets))

CFLAGS := -Wall -ggdb3

all:  $(executables)

# Turn any .c file into a .out file
.c.out:
	$(CC) $< $(CFLAGS) -o $@

.PHONY: clean

clean:
	$(RM) $(executables)

