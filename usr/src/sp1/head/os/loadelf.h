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

#ifndef _OS_LOADELF_H_
#define _OS_LOADELF_H_ 1

#include <sys/types.h>
#include <sys/status.h>
#include <os/membox.h>
#include <stdbool.h>

#define PHDR_MAX 32

/*
 * Represents a loaded range
 *
 * @base_pma: Physical base address range
 * @base_vma: Virtual base of address range
 * @length: Total length from base
 */
struct elf_range {
    uintptr_t base_pma;
    uintptr_t base_vma;
    size_t length;
};

/*
 * Represents a loaded ELF file
 *
 * @entry:  Program entrypoint
 * @membox: Memory box for allocated image
 * @range:  Range list
 * @range_count: Number of ranges total
 */
struct loaded_elf {
    uintptr_t entry;
    struct membox membox;
    struct elf_range range[PHDR_MAX];
    uint8_t range_count;
};

/*
 * Check if a virtual range is good
 *
 * @elf: Descriptor of ELF to check
 * @vbase: Virtual base to check
 * @length: Length from base
 *
 * Returns true if the range is valid
 */
bool elf_vrange_good(struct loaded_elf *elf, uintptr_t vbase, size_t length);

/*
 * Load an ELF binary
 *
 * @data:  Data to load
 * @res:   Result is written here
 */
status_t elf64_load(void *data, struct loaded_elf *res);

#endif  /* !_OS_LOADELF_H_ */
