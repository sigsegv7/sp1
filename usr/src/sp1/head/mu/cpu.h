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

#ifndef _MU_CPU_H_
#define _MU_CPU_H_ 1

#include <sys/types.h>
#include <machine/mcb.h>    /* shared */

/*
 * MI processor specific information
 *
 * @id:   Processor ID
 * @mcb:  Machine core block
 */
struct cpu_info {
    uint8_t id;
    struct mcb mcb;
};

/*
 * Routine used early on to initialize critical processor
 * features and gather information about the processor so
 * that your machine is OwOing smoothly.
 */
void mu_cpu_preinit(struct cpu_info *ci);

#endif  /* !_MU_CPU_H_ */
