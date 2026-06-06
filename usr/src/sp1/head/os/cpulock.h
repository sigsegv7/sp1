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

#ifndef _OS_CPULOCK_H_
#define _OS_CPULOCK_H_ 1

#include <sys/types.h>
#include <stdbool.h>

typedef size_t cpu_lock_t;

/*
 * Acquire a CPU lock
 *
 * @lock: CPU lock to acquire
 * @irqmut: If true, mutuate the IRQ state
 */
void cpu_lock_acquire(cpu_lock_t *lock, bool irqmut);

/*
 * Relase a CPU lock
 *
 * @lock: Lock to relase
 */
void cpu_lock_release(cpu_lock_t *lock);

#endif  /* !_OS_CPULOCK_H_ */

