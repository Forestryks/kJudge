# kJudge

kJudge is a kernel-based sandbox for contest management systems. It is intended to safely execute untrusted third-party code. This is achieved by forbidding potentially harmful system calls and specifying time and memory limits on the program to be executed.

## Installation
*To easily prepare development environment run `kernel/setup.sh`*

1. Change to the `kernel/` directory

```
cd kernel/
```

2. Download the proper Linux kernel source version (`4.14.88` for the moment)

```
wget https://cdn.kernel.org/pub/linux/kernel/v4.x/linux-4.14.88.tar.xz
```

3. Untar the Linux kernel source archive

```
tar -xf linux-4.14.88.tar.xz
```

4. Change to the Linux kernel source directory

```
cd linux-4.14.88
```

5. Apply the kjudge kernel patch

```
patch -p1 < ../kjudge-linux-4.14.88.patch
```

6. Now you are to build the kernel in any preferred way. If you have no idea how this is done see the **Building kernel** section

7. After booting to the new kernel verify whether the kernel is patched

```
uname -r
# output: 4.14.88-kjudge
```

8. Copy the header file to /usr/include
```
cp include/kjudge.h /usr/include/kjudge.h
```

9. *(optionally)* To run tests change to the `tests/` directory and run `runTests.py`

## Usage
TODO

## Building kernel
Please note that this is a universal method for building the kernel, but it may not work on some distributions. If so, you are to follow your system specific instructions which unfortunately are not given here.


1. Copy the existing config file
```
cp -v /boot/config-`uname -r` .config
```

2. Install required tools for building the kernel
- Debian/Ubuntu Linux

```
sudo apt-get install build-essential libncurses-dev bison flex libssl-dev libelf-dev
```

- CentOS/RHEL/Oracle/Scientific Linux

```
sudo yum groupinstall "Development Tools"
sudo yum install ncurses-devel bison flex elfutils-libelf-devel openssl-devel
```

- Fedora Linux

```
sudo dnf group install "Development Tools"
sudo dnf install ncurses-devel bison flex elfutils-libelf-devel openssl-devel
```

3. Configure the kernel (optional)

```
make menuconfig
```

4. Compile the kernel

```
N=6
make -jN
sudo make modules_install
sudo make install
```

5. Update grub config (*Only for CentOS/RHEL/Oracle/Scientific/Fedora Linux*)
```
sudo grub2-mkconfig -o /boot/grub2/grub.cfg
sudo grubby --set-default /boot/vmlinuz-4.14.88-kjudge
grubby --info=ALL | more
grubby --default-index
grubby --default-kernel
```

6. Now reboot into the new kernel and verify whether the kernel is patched
```
uname -r
# output: 4.14.88-kjudge
```
