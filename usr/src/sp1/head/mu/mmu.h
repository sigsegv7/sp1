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

#include <sys/status.h>
#include <sys/mman.h>

/*
 * Each running SP1 process is to have a virtual fuck region
 * which logically isolates their address space with the help
 * of the platform memory management unit.
 */
#include <machine/vfr.h>    /* shared; virtual fuck region~ */

/*
 * Represents valid page sizes that can be used when
 * creating mappings
 */
typedef enum {
    PAGESIZE_4K,
    PAGESIZE_2M,
    PAGESIZE_1G
} pagesize_t;

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

/*
 * Create a virtual to physical mapping within a
 * specific virtual fuck region
 *
 * @vfr:  Virtual fuck region to map within
 * @vma:  Virtual memory address to map
 * @pma:  Physical memory address to map to
 * @prot: Protection flags
 * @pagesize_t: Pagesize to map
 */
status_t mu_mmu_map(
    struct mmu_vfr *vfr, uintptr_t vma,
    uintptr_t pma, int prot,
    pagesize_t ps
);

/*
 * Destroy a virtual memory mapping in the virtual
 * fuck region
 *
 * @vfr: Virtual fuck region to unmap within
 * @vma: Virtual memory address to unmap
 * @ps:  Pagesize of address to unmap
 */
status_t mu_mmu_unmap(
    struct mmu_vfr *vfr, uintptr_t vma,
    pagesize_t ps
);

/*
 * Fork a VFR and clear out the lower half
 *
 * @vfr: Virtual fuck region to fork
 * @res: Virtual fuck region result written here
 */
status_t mu_mmu_forkvfr(struct mmu_vfr *vfr, struct mmu_vfr *res);

#endif  /* !_MU_MMU_H_ */
