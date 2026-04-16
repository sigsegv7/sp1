/*
 * Copyright (c) 2026, Mirocom Laboratories
 * All rights reserved.
 *
 * The following sources are CONFIDENTIAL and PROPRIETARY
 * property of Mirocom Laboratories. Unauthorized copying,
 * use, distribution or modification of this file, in whole
 * and in part, is strictly prohibited without the prior written
 * consent from Mirocom Laboratories.
 */

OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)
ENTRY(_start)

PHDRS
{
    text          PT_LOAD    FLAGS((1 << 0) | (1 << 2)) ;
    rodata        PT_LOAD    FLAGS((1 << 2)) ;
    data          PT_LOAD    FLAGS((1 << 1) | (1 << 2)) ;
}

SECTIONS
{
    . = 0xFFFFFFFF80000000;

    .text : {
        *(.text .text.*)
    } :text

    . += CONSTANT(MAXPAGESIZE);

    .rodata : {
        *(.rodata .rodata.*)
    } :rodata

    . += CONSTANT(MAXPAGESIZE);

    .data : {
        *(.data)
    } :data

    .bss : {
        *(COMMON)
        *(.bss .bss.*)
    } :data

    . += CONSTANT(MAXPAGESIZE);

    /DISCARD/ : {
        *(.eh_frame .eh_frame.*)
        *(.note .note.*)
    }
}
