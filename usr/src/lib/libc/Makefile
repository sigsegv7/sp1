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

CFILES = $(shell find . -name "*.c")
ASMFILES = $(shell find crt/$(ARCH_TARGET) -name "*.S")
ASMOFILES = $(ASMFILES:.S=.S.o)
OFILES = $(CFILES:.c=.o)

CC = $(SYS_CC)
CFLAGS = $(SYS_CFLAGS) -I../../head -D_SP1_SOURCE

.PHONY: all
all: $(OFILES) $(ASMOFILES)

%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $@

%.S.o: %.S
	$(CC) -c $< $(CFLAGS) -o $@
