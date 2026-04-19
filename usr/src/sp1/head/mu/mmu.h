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

#ifndef _MU_MMU_H_
#define _MU_MMU_H_ 1

/*
 * Each running SP1 process is to have a virtual fuck region
 * which logically isolates their address space with the help
 * of the platform memory management unit.
 */
#include <machine/vfr.h>    /* shared; virtual fuck region~ */

/*
 * Obtain the current VFR in-use
 *
 * @res: Result is written here
 */
void mu_mmu_readvfr(struct mmu_vfr *res);

/*
 * Write a new VFR to the processor
 *
 * @vfr: Virtual fuck region to set
 */
void mu_mmu_writevfr(struct mmu_vfr *vfr);

#endif  /* !_MU_MMU_H_ */
