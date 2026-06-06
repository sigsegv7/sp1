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

/*
 * Represents a loaded ELF file
 *
 * @entry:  Program entrypoint
 * @membox: Memory box for allocated image
 */
struct loaded_elf {
    uintptr_t entry;
    struct membox membox;
};

/*
 * Load an ELF binary
 *
 * @data:  Data to load
 * @res:   Result is written here
 */
status_t elf64_load(void *data, struct loaded_elf *res);

#endif  /* !_OS_LOADELF_H_ */
