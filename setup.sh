#!/bin/bash
wget https://cdn.kernel.org/pub/linux/kernel/v4.x/linux-4.14.88.tar.xz
tar -xf linux-4.14.88.tar.xz
cp linux-4.14.88 linux-4.14.88-kjudge -r
cd linux-4.14.88-kjudge
patch -p1 < ../kjudge-linux-4.14.88.patch
