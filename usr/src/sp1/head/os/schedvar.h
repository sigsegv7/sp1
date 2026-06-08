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

#ifndef _OS_SCHEDVAR_H_
#define _OS_SCHEDVAR_H_ 1

#include <sys/types.h>
#include <sys/queue.h>
#include <os/task.h>
#include <os/cpulock.h>

/*
 * Represents a scheduler run queue
 *
 * @lock: Lock protecting the run queue
 * @runq: Actual run queue
 */
struct sched_runq {
    cpu_lock_t lock;
    TAILQ_HEAD(, task) runq;
};

#endif  /* !_OS_SCHEDVAR_H_ */
