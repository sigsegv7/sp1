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

#include <errno.h>
#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>
#include "cumc/state.h"

int
cumc_state_init(const char *input_path, struct cumc_state *res)
{
    if (input_path == NULL || res == NULL) {
        errno = EINVAL;
        return -1;
    }

    res->in_fd = open(input_path, O_RDONLY);
    if (res->in_fd < 0) {
        return -1;
    }

    return 0;
}

void
cumc_state_destroy(struct cumc_state *state)
{
    if (state == NULL) {
        return;
    }

    close(state->in_fd);
    state->in_fd = -1;
}
