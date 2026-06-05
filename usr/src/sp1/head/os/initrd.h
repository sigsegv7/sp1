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

#ifndef _OS_INITRD_H_
#define _OS_INITRD_H_ 1

#include <sys/types.h>
#include <sys/status.h>

/*
 * Represents an initial ramdisk file
 *
 * @data: Actual raw data
 * @sz:   Byte count
 */
struct initrd_file {
    void *data;
    size_t sz;
};

/*
 * Lookup a file from the initial ramdisk
 *
 * @path:  Path to lookup
 * @res:   Result is written here
 */
status_t initrd_lookup(const char *path, struct initrd_file *res);

/*
 * Initialize the initial ramdisk
 */
void initrd_init(void);

#endif  /* !_OS_INITRD_H_ */
