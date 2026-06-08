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

#include <machine/tss.h>
#include <os/knot.h>
#include <mu/param.h>
#include <mu/cpu.h>
#include <mm/kalloc.h>
#include <mm/physmem.h>
#include <mm/vm.h>
#include <string.h>

static void
tss_alloc(struct cpu_info *ci)
{
    struct mcb *mcb;
    const size_t STACK_SIZE = 0x1000;
    struct tss_entry *tss;
    static uintptr_t rsp0_base, rsp0;

    if (ci == NULL) {
        return;
    }

    mcb = &ci->mcb;
    if (mcb->tss == NULL) {
        tss = kalloc(sizeof(*tss));

        if (tss == NULL) {
            knot("failed to alloc TSS\n");
        }

        memset(tss, 0, sizeof(*tss));
        rsp0_base = mm_physmem_alloc(STACK_SIZE / PAGESIZE);
        rsp0_base = (uintptr_t)pma_to_vma(rsp0_base);

        if (rsp0_base == 0) {
            knot("could not allocate rsp0base\n");
        }

        rsp0 = rsp0_base + STACK_SIZE;
        tss->rsp0_low = rsp0 & 0xFFFFFFFF;
        tss->rsp0_high = (rsp0 >> 32) & 0xFFFFFFFF;
        mcb->tss = tss;
    }
}


void
write_tss(struct cpu_info *ci, struct tss_desc *desc)
{
    struct mcb *mcb;
    struct tss_entry *tss;
    uintptr_t tss_base;

    if (ci == NULL || desc == NULL) {
        return;
    }

    mcb = &ci->mcb;
    tss_alloc(ci);

    tss_base = (uintptr_t)mcb->tss;
    desc->seglimit = sizeof(struct tss_entry);
    desc->p = 1;        /* Must be present to be valid! */
    desc->g = 0;        /* Granularity -> 0 */
    desc->avl = 0;      /* Not used */
    desc->dpl = 0;      /* Descriptor Privilege Level -> 0 */
    desc->type = 0x9;   /* For TSS -> 0x9 (0b1001) */

    desc->base_lo16 = tss_base & 0xFFFF;
    desc->base_mid8 = (tss_base >> 16) & 0xFF;
    desc->base_hi_mid8 = (tss_base >> 24) & 0xFF;
    desc->base_hi32 = (tss_base >> 32) & 0xFFFFFFFF;

    tss = mcb->tss;
    tss->io_map = 0xFF;    /* Disallow ring 3 port I/O  */
}
