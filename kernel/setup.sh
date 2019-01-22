#!/bin/bash
VERSION=4.19.16
if [ ! -f linux-${VERSION}.tar.xz ]; then
    wget https://cdn.kernel.org/pub/linux/kernel/v4.x/linux-${VERSION}.tar.xz
fi
tar -xf linux-${VERSION}.tar.xz
cp linux-${VERSION} linux-${VERSION}-kjudge -r
cd linux-${VERSION}-kjudge
patch -p1 < ../kjudge-linux-${VERSION}.patch
