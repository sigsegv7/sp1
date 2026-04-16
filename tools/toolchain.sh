#!/bin/sh
#
# Copyright (c) 2026, Mirocom Laboratories
# All rights reserved.
#
# The following sources are CONFIDENTIAL and PROPRIETARY
# property of Mirocom Laboratories. Unauthorized copying,
# use, distribution or modification of this file, in whole
# and in part, is strictly prohibited without the prior written
# consent from Mirocom Laboratories.
#

TARGET=x86_64-pc-mirocom
MAKE=make

mkdir -p var/
mkdir -p var/cc/root/usr/include/
mkdir -p var/cc/root/usr/bin/

# Don't build again if the lock exists
if [ -f var/cc/.lock ]; then
    echo "var/cc/.lock exists, skipping toolchain build"
    exit 1
fi

pushd var/cc/toolchain
bash build.sh

mkdir -p gcc
pushd gcc

# Configure gcc
../gcc-patched/configure --target=$TARGET \
    --prefix=$(pwd) --with-sysroot=$(pwd)/../../root/ \
    --disable-nls --enable-languages=c --disable-multilib

# Build gcc
$MAKE all-gcc
$MAKE install-gcc

# Lock the directory
popd
popd
touch var/cc/.lock
