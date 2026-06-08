/*
 * Copyright (c) 2026, Mirocom Laboratories
 * All rights reserved.
 *
 * The following sources are CONFIDENTIAL and PROPRIETARY
 * property of Mirocom Laboratories. Unauthorized copying,
 * use, distrubution or modification of this file, in whole
 * and in part, is strictly prohibited without the prior written
 * consent from Mirocom Laboratories.
 */

    .text
    .globl _start
    .extern
_start:
    xor %rbp, %rbp  /* Denote end of callstack */
    call __libc_init

1:  pause
    jmp 1b
