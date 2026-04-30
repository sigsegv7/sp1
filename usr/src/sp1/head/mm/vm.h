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

#ifndef _MM_VM_H_
#define _MM_VM_H_ 1

#include <sys/types.h>
#include <sys/param.h>
#include <os/bpt.h>
#include <mu/mmu.h>

/*
 * Represents a virtual memory mapping that can be made
 *
 * @ps:       Pagesize
 * @vma_base: Virtual memory base
 * @pma_base: Physical memory base
 * @length:   Number of bytes to map
 *
 * XXX: `pma_base' is unused when unmapping regions
 */
struct vm_map {
    pagesize_t ps;
    uintptr_t vma_base;
    uintptr_t pma_base;
    size_t length;
};

/*
 * Macros used to convert physical to virtual addresses
 * and vice versa.
 */
#define pma_to_vma(pma) \
    PTR_OFFSET((void *)((uintptr_t)pma), bpt_kload_base())
#define vma_to_pma(vma) \
    (uintptr_t)PTR_NOFFSET(vma, bpt_kload_base())

/*
 * Create a virtual memory mapping
 *
 * @vfr:     Virtual fuck region to map within
 * @mapping: Mapping to create
 */
status_t mm_vm_map(struct mmu_vfr *vfr, struct vm_map *mapping, int prot);

/*
 * Destroy a virtual memory mapping
 *
 * @vfr:  Virtual fuck region to unmap within
 * @mapping: Mapping to destroy
 */
status_t mm_vm_unmap(struct mmu_vfr *vfr, struct vm_map *mapping);

/*
 * Initialize the virtual memory management
 */
void mm_vm_init(void);

#endif  /* !_MM_VM_H_ */
