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
OFILES = $(CFILES:.c=.o)

CC = $(SYS_CC)
CFLAGS = $(SYS_CFLAGS) -I../../head -D_SP1_SOURCE
OUTPUT_FILE = ../../data/boot/init.sys

CFLAGS = 			\
	-I../../head    \
	$(SYS_CFLAGS)

.PHONY: all
all: $(OUTPUT_FILE)

.PHONY: $(OUTPUT_FILE)
$(OUTPUT_FILE): $(OFILES)
	ld $(OFILES) -T../link.ld -o $@

%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $@
