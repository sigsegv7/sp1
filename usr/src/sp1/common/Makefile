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

CFILES = $(shell find os/ -name "*.c")
OFILES = $(CFILES:.c=.o)

CC = ../../../../$(SYS_CC)

CFLAGS = 			\
	$(SYS_CFLAGS)   \
	-D_KERNEL		\
	-MMD			\
	-DPRINTF_DISABLE_SUPPORT_PTRDIFF_T \
	-DPRINTF_DISABLE_SUPPORT_FLOAT

.PHONY: all
all: $(OFILES)

%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $@
