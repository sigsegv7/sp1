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

#define pr_trace(fmt, ...) \
    printf("irqchip: " fmt, ##__VA_ARGS__)

/* Online capable */
#define LAPIC_ONLCAP BIT(1)

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
            break;
        case APIC_TYPE_IO_APIC:
            ioapic = (struct ioapic *)hdr;
            irqchip_print_ioapic(ioapic);
            break;
        }

        cur += hdr->length;
    }
}
