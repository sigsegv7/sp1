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
#include <machine/cpuid.h>
#include <machine/idt.h>
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
cpu_print_info(struct cpu_info *ci)
{
    static bool once = false;
    struct mcb *mcb;

    if (once || ci == NULL) {
        return;
    }

    mcb = &ci->mcb;
    pr_trace("pg.nx       : yes\n");
    pr_trace("cpu.model   : %x\n", mcb->model_id);
    pr_trace("cpu.family  : %x\n", mcb->family_id);
    once = true;
}

static void
cpu_get_info(struct cpu_info *ci)
{
    struct mcb *mcb;
    uint32_t eax, unused;
    uint8_t model_low, model_high;
    uint8_t family_low, family_high;

    if (ci == NULL) {
        return;
    }

    mcb = &ci->mcb;

    /* Get processor info and feature bits */
    __cpuid(0x01, eax, unused, unused, unused);

    /* Obtain the model ID */
    model_low  = (eax >> 4) & 0xF;
    model_high = (eax >> 16) & 0xF;
    mcb->model_id = (model_high << 4) | model_low;

    /* Obtain the family ID */
    family_low  = (eax >> 8)  & 0xF;
    family_high = (eax >> 20) & 0xFF;
    mcb->family_id = (family_high << 4) | family_low;
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

    /* Obtain some processor information */
    cpu_get_info(ci);

    /* Log out some information */
    cpu_print_info(ci);

    /* Set interrupt vectors */
    md_set_vectors();

    /* Load the IDT */
    md_idt_load();
}
