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

#include <os/knot.h>
#include <lib/printf.h>
#include <mu/cpu.h>
#include <stdarg.h>

/*
 * Even though we can assume that the stack is safe to use upon
 * entry of __knot(), we still want to avoid using it for stability
 * and security reasons. Globals go here.
 */
static char knotbuf[256];
static va_list ap;

__no_return void
__knot(const char *fmt, ...)
{
    va_start(ap, fmt);
    vsnprintf(knotbuf, sizeof(knotbuf), fmt, ap);

    printf("\033[31;40m*******************************************\n");
    printf("ah!~ fuck, i've been knotted~ @.@\n");
    printf("knot: %s\n", knotbuf);

    for (;;) {
        mu_cpu_intoff();
        mu_cpu_halt();
    }
}
