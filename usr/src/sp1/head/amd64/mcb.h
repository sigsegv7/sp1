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

#ifndef _MACHINE_MCB_H_
#define _MACHINE_MCB_H_ 1

#include <sys/types.h>

/*
 * The machine core block contains MD processor
 * specific information
 *
 * @model_id:       Processor model ID
 * @family_id:      Processor family ID
 * @lapic_mmio:     xAPIC MMIO base
 * @x2apic_enabled: If set, x2APIC is enabled
 */
struct mcb {
    uint8_t model_id;
    uint16_t family_id : 12;
    void *lapic_mmio;
    uint8_t x2apic_enabled : 1;
};

#endif  /* !_MACHINE_MCB_H_ */
