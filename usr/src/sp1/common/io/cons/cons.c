/*
 * Copyright (c) 2026, Mirocom Laboratories
 * All rights reserved.
 *
 * The following sources are CONFIDENTIAL and PROPRIETARY
 * property of Mirocom Laboratories. Unauthorized copying,
 * use, distrubution or modification of this file, in whole
 * and in part, is strictly prohibited without the prior written
 * consent from Mirocom Laboratories.
 */

#include <io/cons/cons.h>
#include <lib/limine.h>
#include "flanterm.h"
#include "flanterm_backends/fb.h"

#define FRAMEBUFFER fb_resp->framebuffers[0]

static struct flanterm_context *ft_ctx = NULL;
static struct limine_framebuffer_response *fb_resp = NULL;
static struct limine_framebuffer_request fb_req = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

void
cons_init(struct cons_attr *attr)
{
    if (attr == NULL) {
        return;
    }

    fb_resp = fb_req.response;
    ft_ctx = flanterm_fb_init(
        NULL,
        NULL,
        FRAMEBUFFER->address,
        FRAMEBUFFER->width,
        FRAMEBUFFER->height,
        FRAMEBUFFER->pitch,
        FRAMEBUFFER->red_mask_size,
        FRAMEBUFFER->red_mask_shift,
        FRAMEBUFFER->green_mask_size,
        FRAMEBUFFER->green_mask_shift,
        FRAMEBUFFER->blue_mask_size,
        FRAMEBUFFER->blue_mask_shift,
        NULL,
        NULL,
        NULL,
        &attr->bg,
        &attr->fg,
        NULL,
        NULL,
        NULL,
        0, 0, 0,
        0, 0, 0, 0
    );
}

void
cons_write(const char *str, size_t len)
{
    if (fb_resp == NULL) {
        return;
    }

    if (str == NULL || len == 0) {
        return;
    }

    flanterm_write(ft_ctx, str, len);
}
