#
# Copyright (c) 2026, Mirocom Laboratories
# All rights reserved.
#
# The following sources are CONFIDENTIAL and PROPRIETARY
# property of Mirocom Laboratories. Unauthorized copying,
# use, distrubution or modification of this file, in wcumc
# and in part, is strictly prohibited without the prior written
# consent from Mirocom Laboratories.
#

CC = gcc
CFILES = $(shell find common/ -name "*.c")
OFILES = $(CFILES:.c=.o)
DFILES = $(CFILES:.c=.d)

CFLAGS =		\
	-Wall		\
	-pedantic	\
	-MMD		\
	-Ihead

.PHONY: all
all: ../../bin/cumc

.PHONY: cumc
../../bin/cumc: $(OFILES)
	$(CC) $^ -o $@

-include $(DFILES)
%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@
