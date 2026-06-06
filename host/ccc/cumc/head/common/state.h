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

#ifndef _COMMON_STATE_H_
#define _COMMON_STATE_H_ 1

/*
 * Represents the compiler state machine
 *
 * @in_fd:  Input file descriptor
 */
struct cumc_state {
    int in_fd;
};

/*
 * Initialize the compiler state machine
 *
 * @input_path:  Input path
 * @res:         Result is written here
 *
 * Returns zero on success
 */
int cumc_state_init(const char *input_path, struct cumc_state *res);

/*
 * Clean up an existing compiler state machine
 *
 * @state: State to destroy
 */
void cumc_state_destroy(struct cumc_state *state);

#endif  /* !_COMMON_STATE_H_ */
