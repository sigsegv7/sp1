#
# Copyright (c) 2026, Mirocom Laboratories
# All rights reserved.
#
# The following sources are CONFIDENTIAL and PROPRIETARY
# property of Mirocom Laboratories. Unauthorized copying,
# use, distrubution or modification of this file, in whole
# and in part, is strictly prohibited without the prior written
# consent from Mirocom Laboratories.
#

CC = gcc
CFILES = $(shell find . -name "*.c")
OFILES = $(CFILES:.c=.o)

CFLAGS =		\
	-Wall		\
	-pedantic

.PHONY: all
all: ../bin/hole

.PHONY: hole
../bin/hole: $(OFILES)
	$(CC) $^ -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@
