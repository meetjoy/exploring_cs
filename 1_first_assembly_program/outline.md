# Part 6 Write your very first mini operating system

## outline preparation
### introduction

objective: at the end of the course, you will finish our very own mini operating system
feature: learn by doing, step by step
0 fundamental required

## Section 1
### write our very first program in assembly
first.s
what is assembly?

### how to translate these code file into something the CPU can understand?
what can translate the assembly into an executable file?
we need an assembler
install virtualbox then ubuntu, then the complier
now we've got an execuable or runnable file

### how to run it?
installing bochs Emulator
run it

### concepts
### tools
### assembly codes

## videos, scripts & resources

### 01 Write our very first program in assembly language

### 02 GNU assembler, Linux Desktop, virtual machine & hypervisor
Hi guys, in last video we have written some x86 assembly code and save it into first.s. 
we have learned that the CPU cannot understand assembly code directly, the CPU can only understand and execute machine code directly. 
Now the question for us is how to turn these assembly code into machine code. the program that can translate assembly code into machine code is called assembler. 

### 03A Installing VirtualBox on macOS
### 03B Installing VirtualBox on Windows

Hi guys, in this video we are going to install VirtualBox. Firstly we search "virtualbox" in the browser, then we come to their official website. I am using Windows now, so click and download the Windows version. Now pause the video, download either the Windows version or the MacOS version for your computer.

Welcome back, now we have downloaded the installer. Click to install, .... finally click finsh. Okay, here we are, that's the hypervisor or the virtual machine manager. In the next video we are going to see how to install Ubuntu. 

### 04 Installing Ubuntu Desktop

Hi guys, in this video we are going to install Ubuntu. Firstly search Ubuntu in the browser then we go to their offical website. Under Downloads, we click the 18.04LTS. Lts stands for the long term service version. 18.04 is the current available latest version. The file is nearly 2G, so it takes a couple of minutes. Now pause the video and do the same on your computer. 
2nd part of the video. Finish.

### 05 Terminal, basic Linux commands, as & binutils
Hi guys, in last video we've installed Ubuntu Desktop. According to our plan we are supposed to install the GNU assembler or GAS in this video. 
For these people who are not familiar with the terminal or command line interface, firstly we go through over some very commonly used linux commands. Let's get started!

What is a terminal? 

graphical interface

3 ways to open the terminal
change text size and colour

pwd, cd, ls, mkdir, touch, mv, clear, tree, apt, sudo, ~, 
Table key
as binutils

https://tutorials.ubuntu.com/tutorial/command-line-for-beginners#8


### 06 Assembly, object file, binary, hexadecimal, disassembly, architecture, i8086

manufacturers

### 07 linking, executable file or runnable, boot, ROM, POST, legacy BIOS, MBR, RAM, 0x7c00, UEFI

### 08 installing emulator Bochs
run our first program using emulator Bochs
Emulator
Install bochs
Run sample system
Run first program
### 09 run our first program using Bochs

### 10 Explaination of first.s: directives, instruction, label, interrupt, legacy BIOS attributes

### 11 Making a bash Script

### 12 summary?
