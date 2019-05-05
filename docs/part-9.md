# Part 9:

## install tools

install gcc install as86 & ld86: sudo apt-get install bin86 install make

## download linux-0.12 source code

[https://github.com/sky-big/Linux-0.12](https://github.com/sky-big/Linux-0.12)

## modify and compile

modify Makefile.header file: -O0 to 0Os modify Make file: line 6 to RAMDISK = -DRAMDISK = 2048 modify ROOT\_DEV=0301 to ROOT\_DEV = FLOPPY

Terminal key in: make

## create the RAMDISK

The following two commands put Kernel\_Image and rootimage\_0.12-fd \(which is a root disk\) in one image dd bs=8192 if=Kernel\_Image of=bootroot-0.12 dd bs=1024 if=rootimage-0.12-fd of=bootroot-0.12 seek=256 root disk downloaded from bottom link of [https://github.com/sky-big/Linux-0.12](https://github.com/sky-big/Linux-0.12)

## run final file bootroot-0.12 in emulator

sudo qemu-system-x86\_64 -drive format=raw,file=bootroot-0.12,index=0,if=floppy -cpu max

## Current problem

shows SVGA error

