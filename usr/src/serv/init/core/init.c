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

#include <sys/cdefs.h>

int
main(void)
{
    const char *msg = "~ slutty init v0.0.1 - halted ~\n";

    __asmv(
        "mov $0x01, %%rax;\n\t"
         "mov %0, %%rdi;\n\t"
         "int $0x80"
        :
        : "r" (msg)
        : "rax", "memory"
    );
    return 0;
}
