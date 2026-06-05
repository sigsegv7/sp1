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

#define _DEFAULT_SOURCE
#include <sys/types.h>
#include <sys/queue.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

#define REF_MAGIC "CUMHOLE"
#define REF_MAGIC_LEN 8

/* Set to 1 for verbose */
#define DEBUG 1

#if DEBUG
#define DTRACE(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
#define DTRACE(...) (void)0
#endif

/*
 * A reference is an entry within the table that refers
 * to a block of data after the reference table.
 *
 * @magic:      Reference magic
 * @path:       File path
 * @data_off:   Data offset
 * @link:       Queue link
 */
struct hole_ref {
    char magic[REF_MAGIC_LEN];
    char *path;
    off_t data_off;
    TAILQ_ENTRY(hole_ref) link;
};

/* Globals */
static char *fill_dirpath = NULL;
static TAILQ_HEAD(, hole_ref) hole_files;

/*
 * Display the help options
 */
static void
help(void)
{
    printf("usage: ./hole [flags]\n");
    printf("[-h]    Display this help menu\n");
    printf("[-f]    Fill hole with directory\n");
}

/*
 * Destroy the hole files table
 */
static void
hole_files_destroy(void)
{
    struct hole_ref *ref, *tmp;

    while ((ref = TAILQ_FIRST(&hole_files)) != NULL) {
        DTRACE("delete %s\n", ref->path);

        free(ref->path);
        tmp = ref;
        TAILQ_REMOVE(&hole_files, ref, link);
        free(tmp);
    }
}

/*
 * Push a file to the hole files table
 */
static int
push_file(const char *path)
{
    struct hole_ref *ref;

    if (path == NULL) {
        return -1;
    }

    ref = malloc(sizeof(*ref));
    if (ref == NULL) {
        printf("fatal: could not allocate ref\n");
        hole_files_destroy();
        return -1;
    }

    memcpy(ref->magic, REF_MAGIC, sizeof(REF_MAGIC));
    ref->path = strdup(path);

    ref->data_off = 0;
    TAILQ_INSERT_TAIL(&hole_files, ref, link);
    return 0;
}

/*
 * Scan a directory to fill the blob
 */
static void
scan_dir(const char *path)
{
    DIR *dir;
    struct dirent *dirent;
    char pathbuf[300];

    if (path == NULL) {
        return;
    }

    dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        return;
    }

    while ((dirent = readdir(dir)) != NULL) {
        if (dirent->d_name[0] == '.') {
            continue;
        }

        snprintf(pathbuf, sizeof(pathbuf), "%s/%s", path, dirent->d_name);
        switch (dirent->d_type) {
        case DT_REG:
            if (push_file(pathbuf) < 0) {
                free(fill_dirpath);
                exit(1);
            }
            break;
        case DT_DIR:
            printf("[d] %s\n", pathbuf);
            scan_dir(pathbuf);
            break;
        }
    }

    closedir(dir);
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

    while ((opt = getopt(argc, argv, "hf:")) != -1) {
        switch (opt) {
        case 'h':
            help();
            return -1;
        case 'f':
            fill_dirpath = strdup(optarg);
            if (fill_dirpath == NULL) {
                printf("fatal: out of memory\n");
                return -1;
            }
            break;
        }
    }

    if (fill_dirpath == NULL) {
        printf("fatal: expected fill directory for the slutty blob...\n");
        help();
        return -1;
    }

    TAILQ_INIT(&hole_files);
    scan_dir(fill_dirpath);
    hole_files_destroy();
    free(fill_dirpath);
    return 0;
}
