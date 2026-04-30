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

#include <sys/types.h>
#include <sys/param.h>
#include <os/knot.h>
#include <lib/printf.h>
#include <machine/irqchip.h>
#include <io/acpi/acpi.h>
#include <io/acpi/tables.h>
#include <mm/vm.h>

#define pr_trace(fmt, ...) \
    printf("irqchip: " fmt, ##__VA_ARGS__)

/* Online capable */
#define LAPIC_ONLCAP BIT(1)

/* I/O APIC */
static struct irqchip ioapic_list[MAX_IOAPIC];
static size_t ioapic_count = 0;

/* Local APIC */
static struct irqchip lapic_list[MAX_LAPIC];
static size_t lapic_count = 0;

/*
 * Add a Local APIC descriptor
 */
static inline void
irqchip_lapic_append(const struct irqchip *irqchip)
{
    if (lapic_count >= MAX_LAPIC) {
        return;
    }

    lapic_list[lapic_count++] = *irqchip;
}

/*
 * Add an I/O APIC descriptor
 */
static inline void
irqchip_ioapic_append(const struct irqchip *irqchip)
{
    if (ioapic_count >= MAX_IOAPIC) {
        return;
    }

    ioapic_list[ioapic_count++] = *irqchip;
}

/*
 * Print information about a Local APIC unit
 */
static inline void
irqchip_print_lapic(struct local_apic *lapic)
{
    static uint16_t log_count = 0;

    if (lapic == NULL) {
        return;
    }

    if ((log_count++) >= 4) {
        pr_trace("....\n");
        return;
    }

    pr_trace("lapic(%d).cpu       : %d\n",
        lapic->apic_id, lapic->processor_id);
}

/*
 * Print information about an I/O APIC unit
 */
static inline void
irqchip_print_ioapic(struct ioapic *ioapic)
{
    if (ioapic == NULL) {
        return;
    }

    pr_trace("ioapic(%d).gsi_base : %d\n",
        ioapic->ioapic_id, ioapic->gsi_base);
    pr_trace("ioapic(%d).mmio     : %p\n",
        ioapic->ioapic_id, ioapic->ioapic_addr);
}

status_t
md_irqchip_init(void)
{
    struct irqchip chip;
    struct acpi_madt *madt;
    struct local_apic *lapic;
    struct ioapic *ioapic;
    struct apic_header *hdr;
    char *cur, *end;

    madt = acpi_query("APIC");
    if (madt == NULL) {
        knot("could not query acpi madt table\n");
    }

    cur = (char *)(madt + 1);
    end = (char *)madt + madt->hdr.length;

    while (cur < end) {
        hdr = (struct apic_header *)cur;

        switch (hdr->type) {
        case APIC_TYPE_LOCAL_APIC:
            lapic = (struct local_apic *)hdr;
            irqchip_print_lapic(lapic);

            chip.mmio = pma_to_vma(madt->lapic_addr);
            chip.apic_id = lapic->apic_id;
            irqchip_lapic_append(&chip);
            break;
        case APIC_TYPE_IO_APIC:
            ioapic = (struct ioapic *)hdr;
            irqchip_print_ioapic(ioapic);

            chip.mmio = pma_to_vma(ioapic->ioapic_addr);
            chip.apic_id = ioapic->ioapic_id;
            irqchip_lapic_append(&chip);
            break;
        }

        cur += hdr->length;
    }
}

struct irqchip *
md_ioapic_index(size_t index)
{
    if (index >= ioapic_count) {
        return NULL;
    }

    return &ioapic_list[index];
}

struct irqchip *
md_lapic_index(size_t index)
{
    if (index >= lapic_count) {
        return NULL;
    }

    return &lapic_list[index];
}
