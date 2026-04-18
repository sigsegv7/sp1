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

#ifndef _OS_BPT_H_
#define _OS_BPT_H_ 1

#include <sys/types.h>
#include <sys/status.h>

/*
 * Memory map entry types
 */
typedef enum {
    MEMORY_USABLE,
    MEMORY_RESERVED,
    MEMORY_ACPI_RECLAIM,
    MEMORY_ACPI_NVS,
    MEMORY_BAD,
    MEMORY_BOOTLOADER,
    MEMORY_KERNEL,
    MEMORY_FRAMEBUFFER,
    MEMORY_ACPI_TABLES
} mem_type_t;

/*
 * Represents a memory map entry
 *
 * @base:   Entry base address
 * @length: Entry length
 * @type:   Entry type
 */
struct mem_entry {
    uintptr_t base;
    size_t length;
    mem_type_t type;
};

/*
 * The protovar stores a list of information passed
 * by the bootloader.
 *
 * @kload_base:  Kernel load base
 */
struct bpt_protovar {
    uintptr_t kload_base;
};

/*
 * Operations associated with a specific boot protocol
 */
struct bpt_ops {
    status_t(*get_protovar)(struct bpt_protovar *res);
    status_t(*mem_entry_i)(size_t index, struct mem_entry *res);
};

/*
 * Obtain the protocol variable structure reference
 *
 * @res: Result is written here
 */
status_t bpt_get_protovar(struct bpt_protovar *res);

/*
 * Obtain a memory map entry by index
 *
 * Returns STATUS_NOT_FOUND on failure
 */
status_t bpt_mem_entry_i(size_t index, struct mem_entry *res);

/*
 * Initialize the boot protocol translation layer
 */
status_t bpt_init(void);

/* Boot protocol specific init funcs */
status_t bpt_init_limine(struct bpt_ops *ops);

#endif  /* !_OS_BPT_H_ */
