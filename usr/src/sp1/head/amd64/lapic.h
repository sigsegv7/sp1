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

#ifndef _MACHINE_LAPIC_H_
#define _MACHINE_LAPIC_H_

#include <sys/status.h>
#include <mu/cpu.h>

#define LAPIC_TIMER_INTVEC 0x81

/*
 * Initialize the Local APIC
 */
status_t md_lapic_init(struct cpu_info *ci);

/*
 * Send an end-of-interrupt to the Local APIC
 */
void lapic_eoi(void);

/*
 * Prepare the Local APIC timer oneshot mode
 */
void lapic_timer_oneshot_us(size_t usec);

#endif  /* !_MACHINE_LAPIC_H_ */
