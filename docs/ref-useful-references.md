# Useful References

## C reference

[https://en.cppreference.com/w/c](https://en.cppreference.com/w/c)

## C keyword: volatile

{% embed url="https://barrgroup.com/Embedded-Systems/How-To/C-Volatile-Keyword" caption="" %}

## C keyword: \#define

{% embed url="https://www.techonthenet.com/c\_language/constants/create\_define.php" caption="" %}

## What is POSIX?

{% embed url="https://en.wikipedia.org/wiki/POSIX" caption="" %}

## Escape sequences in C

{% embed url="https://en.cppreference.com/w/c/language/escape" %}

## Download the run Bochs package

{% embed url="http://oldlinux.org/Linux.old/bochs/linux-0.12-080324.zip" caption="" %}

Play on Windows, install Bochs-2.3.6.exe, copy bochsrc-0.12-fd.bxrc,diska.img, rootimage-0.12-fd,debug.bat and bootimage-0.12-fd. Double click bochsrc-0.12-fd.bxrc to run.

## install tools

install gcc install as86 & ld86: sudo apt-get install bin86 install make

## Way 1: complie sky-big: download linux-0.12 source code

[https://github.com/sky-big/Linux-0.12](https://github.com/sky-big/Linux-0.12) modify and compile:modify Makefile.header file: -O0 to 0Os modify Make file: line 6 to RAMDISK = -DRAMDISK = 2048 modify ROOT\_DEV=0301 to ROOT\_DEV = FLOPPY;Terminal key in: make

## Way 2: complie original \(modify first\)

## draft: create the RAMDISK

The following two commands put Kernel\_Image and rootimage\_0.12-fd \(which is a root disk\) in one image dd bs=8192 if=Kernel\_Image of=bootroot-0.12 dd bs=1024 if=rootimage-0.12-fd of=bootroot-0.12 seek=256 root disk downloaded from bottom link of [https://github.com/sky-big/Linux-0.12](https://github.com/sky-big/Linux-0.12)

## Draft: run final file bootroot-0.12 in emulator

sudo qemu-system-x86\_64 -drive format=raw,file=bootroot-0.12,index=0,if=floppy -cpu max

