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
ASMOFILES = $(ASMFILES:.S=.S.o)

MISC_OFILES = $(shell find ../../ -name "*.o")

CC = 				\
	../../../../$(SYS_CC)

LD =				\
	../../../../$(SYS_LD)

CFLAGS = 			\
	$(SYS_CFLAGS)

.PHONY: all
all: $(ASMOFILES)
	$(LD) -Tconf/link.ld $(MISC_OFILES) -o ../../../../sp1.sys

%.S.o: %.S
	$(CC) -c $(CFLAGS) $< -o $@
