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

ASMFILES = $(shell find . -name "*.S")
CFILES = $(shell find . -name "*.c")

ASMOFILES = $(ASMFILES:.S=.S.o)
OFILES = $(CFILES:.c=.o)

MISC_OFILES = $(shell find ../ -name "*.o")
LIB_OFILES = $(shell find ../../lib/ -name "*.o")

CC = 				\
	../../../../$(SYS_CC)

LD =				\
	../../../../$(SYS_LD)

CFLAGS = 			\
	$(SYS_CFLAGS)   \
	-I../../head    \
	-I../../sp1/head\
	-D_KERNEL

.PHONY: all
all: $(OFILES) $(ASMOFILES)
	$(LD) 			   \
		-Tconf/link.ld \
		$(MISC_OFILES) \
		$(LIB_OFILES)  \
		-o ../../../../sp1.sys

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

%.S.o: %.S
	$(CC) -c $(CFLAGS) $< -o $@
