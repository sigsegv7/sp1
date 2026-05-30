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

CC_PREFIX =
ARCH_TARGET = amd64
BOOT_PROTO = limine

ifeq ($(ARCH_TARGET), amd64)
	CC_TARGET = x86_64-unknown-elf
else
	CC_TARGET =
endif

CC = \
	clang
LD = \
	 ld
AR = \
	var/cc/toolchain/build-binutils/bin/$(CC_PREFIX)-ar

SYS_CFLAGS =			\
	-nostdlib			\
	-nostdinc			\
	-ffreestanding		\
	-fexceptions		\
	-target $(CC_TARGET)\
	-mcmodel=kernel		\
	-Wno-attributes		\
	-fno-stack-protector\
	-D_SP1_MULTICORE    \

ifeq ($(ARCH_TARGET),x86_64)
	SYS_CFLAGS += 		\
		-mno-sse		\
		-mno-sse2		\
		-mno-sse3		\
		-mno-avx		\
		-mno-avx2		\
		-mno-80387		\
		-mno-3dnow		\
		-mno-mmx
endif

PASSDOWN_ARGS = \
	ARCH=$(ARCH_TARGET)		\
	SYS_CC=$(CC)			\
	SYS_LD=$(LD)			\
	SYS_CFLAGS="$(SYS_CFLAGS)" \
	BOOT_PROTO="\"$(BOOT_PROTO)\""
