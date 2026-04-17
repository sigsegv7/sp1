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

#include <mu/cpu.h>
#include <lib/printf.h>
#include <stdbool.h>

#define pr_trace(fmt, ...) \
    printf("cpu: " fmt, ##__VA_ARGS__)

/* From locore.S - low level processor init */
extern void md_cpu_init(void);

/*
 * Print information about the bootstrap
 * processor on early startup
 */
static void
cpu_print_info(void)
{
    static bool once = false;

    if (once) {
        return;
    }

    pr_trace("pg.nx : yes\n");
    once = true;
}

void
mu_cpu_preinit(struct cpu_info *ci)
{
    if (ci == NULL) {
        return;
    }

    /* Default to an ID of zero [BSP] */
    ci->id = 0;

    /* Initialize control registers and such */
    md_cpu_init();

    /* Log out some information */
    cpu_print_info();
}
