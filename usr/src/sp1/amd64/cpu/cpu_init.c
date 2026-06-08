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
#include <os/schedvar.h>
#include <machine/cpuid.h>
#include <machine/idt.h>
#include <machine/lapic.h>
#include <machine/msr.h>
#include <machine/mcb.h>
#include <stdbool.h>
#include <string.h>

#define pr_trace(fmt, ...) \
    printf("cpu: " fmt, ##__VA_ARGS__)

/* From locore.S - low level processor init */
extern void md_cpu_init(void);
extern struct gdt_entry g_GDT[GDT_ENTRY_COUNT];

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
    pr_trace("pg.nx            : yes\n");
    pr_trace("cpu.model        : %x\n", mcb->model_id);
    pr_trace("cpu.family       : %x\n", mcb->family_id);
    once = true;
}

/*
 * Obtain the processor vendor
 */
static void
cpu_get_vendor(void)
{
    uint32_t ebx, edx, ecx, unused;
    char vendor[13];

    /* Obtain the manufacturer ID */
    __cpuid(0x0, unused, ebx, ecx, edx);

    /* Dword 0 */
    vendor[0] = ebx & 0xFF;
    vendor[1] = (ebx >> 8) & 0xFF;
    vendor[2] = (ebx >> 16) & 0xFF;
    vendor[3] = (ebx >> 24) & 0xFF;

    /* Dword 1 */
    vendor[4] = edx & 0xFF;
    vendor[5] = (edx >> 8) & 0xFF;
    vendor[6] = (edx >> 16) & 0xFF;
    vendor[7] = (edx >> 24) & 0xFF;

    /* Dword 1 */
    vendor[8] = ecx & 0xFF;
    vendor[9] = (ecx >> 8) & 0xFF;
    vendor[10] = (ecx >> 16) & 0xFF;
    vendor[11] = (ecx >> 24) & 0xFF;
    vendor[12] = '\0';
    pr_trace("cpu.manufacturer : %s\n", vendor);
}

/*
 * Obtain processor specific information
 */
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
    cpu_get_vendor();

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
    struct mcb *mcb;

    if (ci == NULL) {
        return;
    }

    mcb = &ci->mcb;
    memcpy(mcb->gdt, g_GDT, sizeof(mcb->gdt));
    mcb->gdtr.limit = sizeof(g_GDT) - 1;
    mcb->gdtr.offset = (uintptr_t)&mcb->gdt;
    gdt_load(&mcb->gdtr);

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

    /* Set current processor */
    md_wrmsr(IA32_GS_BASE, (uintptr_t)ci);

    /* Initialize the run queue */
    ci->runq.lock = 0;
    TAILQ_INIT(&ci->runq.runq);
}

struct cpu_info *
mu_this_cpu(void)
{
    return (struct cpu_info *)md_rdmsr(IA32_GS_BASE);
}

void
mu_cpu_postinit(struct cpu_info *ci)
{
    if (ci == NULL) {
        return;
    }

    /* Initialize the Local APIC unit */
    md_lapic_init(ci);
}
