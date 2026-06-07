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

#ifndef _MACHINE_PCB_H_
#define _MACHINE_PCB_H_ 1

#include <machine/vfr.h>

/*
 * Represents the process control block which holds
 * low-level information regarding a specific process
 *
 * @vfr: Virtual fuck region
 */
struct pcb {
    struct mmu_vfr vfr;
};

#endif  /* !_MACHINE_PCB_H_ */
