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

/*
 * Initialize the Local APIC
 */
status_t md_lapic_init(struct cpu_info *ci);

#endif  /* !_MACHINE_LAPIC_H_ */
