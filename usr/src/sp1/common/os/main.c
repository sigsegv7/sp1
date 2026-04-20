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

#include <io/cons/cons.h>
#include <os/bpt.h>
#include <os/knot.h>
#include <lib/printf.h>
#include <mu/cpu.h>
#include <mm/physmem.h>
#include <mm/vm.h>

#define KERNEL_VERSION "0.0.1"

/* Bootstrap processor */
static struct cpu_info bsp;

/* Root console attribute */
static struct cons_attr cons_attr = {
    .fg = 0x808080,
    .bg = 0x000000
};

static void
boot_banner(void)
{
    printf("-- sp1/amd64 %s --\n", KERNEL_VERSION);
    printf("yiff!~ preparing syspaw for you!... <3\n");
}

void
main(void)
{
    /* Initialize the console */
    cons_init(&cons_attr);

    /* Write the boot console */
    boot_banner();

    /* Pre-init the processor */
    mu_cpu_preinit(&bsp);

    /* Initialize boot protocol translation layer */
    bpt_init();

    /* Initialize physical memory management */
    mm_physmem_init();

    /* Initialize virtual memory management */
    mm_vm_init();

    printf("[*] sp1 is pre-alpha\n");
    printf("[*] knotting kernel...\n");
    knot("end of kernel reached - halting\n");
}
