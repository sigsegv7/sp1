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

#include <stdio.h>
#include <unistd.h>
#include "cumc/state.h"

void
help(void)
{
    printf("usage: cumc [flags]\n");
    printf("[-h]    Display this help menu\n");
}

/*
 * Begin compiling an input source file
 *
 * @input_path: Path of input source file
 *
 * Returns zero on success
 */
static int
compile_run(const char *input_path)
{
    struct cumc_state state;

    if (input_path == NULL) {
        return -1;
    }

    if (cumc_state_init(input_path, &state) < 0) {
        perror("cumc_state_init");
        return -1;
    }

    cumc_state_destroy(&state);
    return 0;
}

int
main(int argc, char **argv)
{
    int opt;

    if (argc < 2) {
        printf("fatal: too few arguments\n");
        help();
        return -1;
    }

    while ((opt = getopt(argc, argv, "h")) != -1) {
        switch (opt) {
        case 'h':
            help();
            return -1;
        }
    }

    while (optind < argc) {
        if (compile_run(argv[optind++]) < 0) {
            break;
        }
    }

    return 0;
}
