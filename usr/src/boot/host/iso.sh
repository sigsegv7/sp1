#!/bin/bash
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

set -e

ISO=../../../sp1.iso
CONFIG=../data/boot/limine.conf
WALLPAPER=../data/boot/wallpaper.jpg
KERNEL=../../../sp1.sys

mkdir -p iso_root/boot/

# Copy the kernel to the ISO root
cp $KERNEL iso_root/boot

# Copy the wallpaper to the ISO root
cp $WALLPAPER iso_root/boot

# Copy boot files
cp $CONFIG stand/limine/limine-bios.sys \
    stand/limine/limine-bios-cd.bin \
    stand/limine/limine-uefi-cd.bin \
    iso_root/

# Generate the ISO
xorriso -as mkisofs -b limine-bios-cd.bin -no-emul-boot -boot-load-size 4 \
        -boot-info-table --efi-boot limine-uefi-cd.bin -efi-boot-part \
		--efi-boot-image --protective-msdos-label iso_root/ -o $ISO 1>/dev/null

stand/limine/limine bios-install $ISO 1>/dev/null
rm -rf iso_root
