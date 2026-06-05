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
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/queue.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdbool.h>

#define REF_MAGIC "CUMHOLE"
#define REF_MAGIC_LEN 8
#define REF_PATH_LEN 256

/* Set to 1 for verbose */
#define DEBUG 0

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
 * @size:       File size
 * @data_off:   Data offset
 * @real_path:  Real path
 * @link:       Queue link
 *
 * XXX: Do not reorder anything ABOVE 'link'
 */
struct hole_ref {
    char magic[REF_MAGIC_LEN];
    char path[REF_PATH_LEN];
    size_t size;
    off_t data_off;
    char *real_path;
    TAILQ_ENTRY(hole_ref) link;
};

/* Globals */
static bool output_override = false;
static char *output_path = "out.hole";
static char *fill_dirpath = NULL;
static size_t file_count = 0;
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
    printf("[-o]    Output blob path\n");
}

/*
 * Destroy the hole files table
 */
static void
hole_files_destroy(void)
{
    struct hole_ref *ref, *tmp;

    while ((ref = TAILQ_FIRST(&hole_files)) != NULL) {
        DTRACE("delete %s\n", ref->real_path);
        free(ref->real_path);

        tmp = ref;
        TAILQ_REMOVE(&hole_files, ref, link);
        free(tmp);
    }
}

/*
 * Used to fix up offsets after
 */
static void
hole_table_fixup(void)
{
    struct stat sb;
    struct hole_ref *ref;
    size_t table_end, offset;
    size_t hdr_size;

    hdr_size = sizeof(struct hole_ref);
    hdr_size -= sizeof(ref->real_path);
    hdr_size -= sizeof(ref->link);
    table_end = file_count * hdr_size;
    offset = table_end;

    printf("* fixing up offsets...\n");
    printf("* table end @ %zx\n", table_end);

    TAILQ_FOREACH(ref, &hole_files, link) {
        if (stat(ref->real_path, &sb) < 0) {
            perror("stat");
            continue;
        }

        ref->size = sb.st_size;
        ref->data_off = offset;

        DTRACE("data_off=%zx\n", ref->data_off);
        offset += ref->size;
    }
}

static void
blob_write(void)
{
    struct hole_ref *ref;
    int fd, ffd;
    void *data;
    size_t fsize, hdr_len;

    fd = open(output_path, O_RDWR | O_CREAT | O_TRUNC, 0777);
    if (fd < 0) {
        perror("open");
        return;
    }

    /*
     * PASS 1):
     *      We are to write the header of each file into the
     *      top of the blob.
     */
    TAILQ_FOREACH(ref, &hole_files, link) {
        hdr_len =  sizeof(*ref);
        hdr_len -= sizeof(ref->real_path);
        hdr_len -= sizeof(ref->link);
        write(fd, ref, hdr_len);
    }

    /*
     * PASS 2):
     *      Now we can write the contents of each file after
     */
    TAILQ_FOREACH(ref, &hole_files, link) {
        ffd = open(ref->real_path, O_RDONLY);
        if (ffd < 0) {
            perror("open[ffd]");
            continue;
        }

        /* Obtain the file size */
        fsize = lseek(ffd, 0, SEEK_END);
        lseek(ffd, 0, SEEK_SET);

        data = mmap(
            NULL,
            fsize,
            PROT_READ,
            MAP_SHARED,
            ffd,
            0
        );

        if (data == NULL) {
            close(ffd);
            perror("mmap");
            continue;
        }

        lseek(fd, SEEK_SET, ref->data_off);
        write(fd, data, fsize);
        munmap(data, fsize);
        close(ffd);
    }

    close(fd);
    return;
}

/*
 * Strip a path of leading '.' and '/' characters
 *
 * @path: Path to strip
 */
static const char *
path_strip(const char *path)
{
    const char *p;

    if (path == NULL) {
        return NULL;
    }

    p = path;
    while (*p == '.' || *p == '/') {
        ++p;
    }

    return p;
}

/*
 * Push a file to the hole files table
 */
static int
push_file(const char *path)
{
    struct hole_ref *ref;
    const char *stripped;

    if (path == NULL) {
        return -1;
    }

    ref = malloc(sizeof(*ref));
    if (ref == NULL) {
        printf("fatal: could not allocate ref\n");
        hole_files_destroy();
        return -1;
    }

    ref->real_path = strdup(path);
    stripped = path_strip(path);

    memcpy(ref->magic, REF_MAGIC, sizeof(REF_MAGIC));
    memcpy(ref->path, stripped, strlen(stripped) + 1);
    ref->data_off = 0;

    TAILQ_INSERT_TAIL(&hole_files, ref, link);
    ++file_count;
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
            printf("[f] %s\n", pathbuf);
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

    while ((opt = getopt(argc, argv, "hf:o:")) != -1) {
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
        case 'o':
            output_override = true;
            output_path = strdup(optarg);
            if (output_path == NULL) {
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

    hole_table_fixup();
    blob_write();

    hole_files_destroy();
    free(fill_dirpath);

    if (output_override) {
        free(output_path);
    }

    return 0;
}
