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

#include <os/sched.h>

void
sched_task_enqueue(struct sched_runq *runq, struct task *task)
{
    if (runq == NULL && task != NULL) {
        return;
    }

    cpu_lock_acquire(&runq->lock, true);
    TAILQ_INSERT_TAIL(&runq->runq, task, runq_link);
    cpu_lock_release(&runq->lock);
}

struct task *
sched_task_dequeue(struct sched_runq *runq)
{
    struct task *task = NULL;

    cpu_lock_acquire(&runq->lock, true);
    if (TAILQ_EMPTY(&runq->runq)) {
        goto done;
    }

    task = TAILQ_FIRST(&runq->runq);
    TAILQ_REMOVE(&runq->runq, task, runq_link);
done:
    cpu_lock_release(&runq->lock);
    return task;
}
