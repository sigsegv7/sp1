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

#ifndef _MACHINE_IRQCHIP_H_
#define _MACHINE_IRQCHIP_H_ 1

#include <sys/status.h>

#define MAX_IOAPIC 8
#define MAX_LAPIC 64

/*
 * @IRQCHIP_NONE: This IRQ chip has no type
 * @IRQCHIP_LAPIC: This IRQ chip is a Local APIC
 * @IRQCHIP_IOAPIC: This IRQ chip is an I/O APIC
 */
typedef enum {
    IRQCHIP_NONE,
    IRQCHIP_LAPIC,
    IRQCHIP_IOAPIC
} irqchip_type_t;

/*
 * Represents a platform interrupt controller
 * chip
 *
 * @type:       IRQ chip type
 * @apic_id:    APIC ID
 * @mmio:       Memory mapped I/O address
 */
struct irqchip {
    irqchip_type_t type;
    uint8_t apic_id;
    void *mmio;
};

/*
 * Initialize platform interrupt controller chips
 */
status_t md_irqchip_init(void);

/*
 * Obtain an I/O APIC descriptor by index
 *
 * @index: Index of entry to obtain
 */
struct irqchip *md_ioapic_index(size_t index);

/*
 * Obtain an Local APIC descriptor by index
 *
 * @index: Index of entry to obtain
 */
struct irqchip *md_lapic_index(size_t index);

#endif  /* !_MACHINE_IRQCHIP_H_ */
