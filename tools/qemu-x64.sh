#!/bin/sh

qemu-system-x86_64 \
    -cdrom sp1.iso \
    --enable-kvm   \
    -m 2G          \
    -monitor stdio \
    -cpu host
