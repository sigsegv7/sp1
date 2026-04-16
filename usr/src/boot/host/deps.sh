#!/bin/sh
#
# Copyright (c) 2026, Mirocom Laboratories
# All rights reserved.
#
# The following sources are CONFIDENTIAL and PROPRIETARY
# property of Mirocom Laboratories. Unauthorized copying,
# use, distrubution or modification of this file, in whole
# and in part, is strictly prohibited without the prior written
# consent from Mirocom Laboratories.
#

if [ ! -d stand/limine ]; then
	git clone https://codeberg.org/Limine/Limine.git --branch=v10.x-binary --depth=1 stand/limine
    make -C stand/limine
fi
