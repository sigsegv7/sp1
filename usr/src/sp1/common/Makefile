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
CFILES += $(shell find io/ -name "*.c")
CFILES += $(shell find bpt/ -name "*.c")
CFILES += $(shell find mm/ -name "*.c")
CFILES += $(shell find ../ext -name "*.c")
CFILES += $(shell find ../lib -name "*.c")

OFILES = $(CFILES:.c=.o)
DFILES = $(CFILES:.c=.d)

CC = clang

CFLAGS = 			\
	$(SYS_CFLAGS)   \
	-I../head		\
	-I../head/target/ \
	-I../../head	\
	-I../ext/flanterm/src/ \
	-D_KERNEL		\
	-MMD			\
	-DPRINTF_DISABLE_SUPPORT_PTRDIFF_T \
	-DPRINTF_DISABLE_SUPPORT_FLOAT     \
	-D_BOOT_PROTO="\"$(BOOT_PROTO)\""

.PHONY: all
all: $(OFILES)

-include $(DFILES)
%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $@
