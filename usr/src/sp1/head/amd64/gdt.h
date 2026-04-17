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

#ifndef _MACHINE_GDT_H_
#define _MACHINE_GDT_H_ 1

/* Kernel code/data */
#define GDT_KCODE  0x08
#define GDT_KDATA  0x10

/* User code/data */
#define GDT_UCODE  0x18
#define GDT_UDATA  0x20

/* Task state segment */
#define GDT_TSS    0x28
#define GDT_TSS_INDEX 0x05

#endif  /* !_MACHINE_GDT_H_ */
