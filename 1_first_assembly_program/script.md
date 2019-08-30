### Video 1 Introduction
Hello everyone, nowdays we are using all kinds of computers everyday: your laptop, Macbook, or your smartphone. 
You must have ever been wondering what happens in the startup stage of your Windows operating system, your MacOS or the mobilephones. 

In this short course, we will firstly introduce what is assembly language and machine language, the correspondence between them. Then we write our very first program in assembly language.

In order to run this program, we need a program called assembler to translate our assembly program to an object file, and then use another program called linker to transfer this object file to the executable file. We are going to do all of these on a Linux Distribution Ubuntu Desktop. 

I will show you how to do all of these step by step. From installing the Linux Desktop, the most commonly used Linux commands, to the use of GNU assembler, GNU linker and objdump. And how to make a script file.

We also cover a bunch of concepts: hexidecimal, disassembly, instruction set architecture, booting, Power on Self Test, Read only memory, legacy BIOS, master booting block, Random access memory, Interrupt and ASCII Character etc..

We will see how compilation of the source code of the Bochs and the installtion, using the classical steps: configue, make, and make install. Eventually we will run our first program on Bochs. 

Follow this course, I gurrantee you will learn the basic usage of lots of linux commands and programs, understand better over the low-level computer technical concepts. And open a door to the world of the operating system and computer science. See you there. 

### Video 2 Write our very first program in assembly language
We learn that there is a strong correspondence between the machine code and the assembly code. Then we write our first program in assembly lanuage.  
x86 assembly language:
https://en.wikipedia.org/wiki/X86_assembly_language
Machine code:
https://en.wikipedia.org/wiki/Machine_code

### Video 3 GNU assembler, Linux Desktop, virtual machine & hypervisor
Introduction of assembler which is used to translate assembly code to machine code. Then gives a way on installing a Linux Desktop if you have not got one. 
https://help.ubuntu.com/stable/ubuntu-help/

### Video 4-5 Installing VirtualBox
### Video 6-7 Installing Ubuntu Desktop on VirtualBox

### Video 8 Terminal, basic Linux commands and binutils
Introduces the most commonly used linux commands. We learn that GNU assembler is part of the Binutils package.
Linux command line:
https://tutorials.ubuntu.com/tutorial/command-line-for-beginners#0

### Video 9 Assembly, object file, binary, hexadecimal, disassembly, architecture and i8086
We get a deep understanding on assembly, object file, binary, disassembly and architecture by using programs as, xxd and objdump.

### Video 10 Linking, executable file or runnable, booting, POST, ROM, legacy BIOS, MBR, RAM, 0x7c00 and UEFI
We get a good understanding on linking, executable file, booting, and legacy BIOS etc..

### Video 11 Installing emulator Bochs-2.6.9 on Ubuntu18.04LTS
Commands used when installing Bochs:
sudo apt install libxcursor-dev libxrandr-dev libxinerama-dev libxi-dev libx11-dev
./configure
make
sudo make install

### Video 12 Run our first program using Bochs
### Video 13 Explain our first assembly language line by line: Interrupt, ASCII
### Video 14 Make a script file to save clicks

### Video 15


### Target of student
most fundamental basics of assembly language
a good understanding of the correspondence between machine code and assembly code
examples of the use of GNU assembler as and GNU linker ld
terminal and most commonly used Linux commands
concepts and installation of emulator Bochs, Linux Desktop, virtual machine and hypervisor
concepts and examples of interrupt, legacy BIOS, booting process, ASCII
fundamental use of script file
example use of classical steps of building and installing software package on Linux
concepts and example of assembly, disassembly & linking
concepts and examples of RAM, ROM, executable file or runnable, object file, instruction set architecture
use of executable file viewer and object file viewer

No. Even friendly to high school (middle school) students.
University students or who are interested in low level principles of computer science

