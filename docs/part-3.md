# Part 3: Start Examining An Operating System

From today, we are diving deeper to see how an operating system works. We start from 0, gradually we get an minimal operating system. 

## Day 1: Boot loader, bootsect.S

### Main functions 

The main process and functions of bootsect.S includes:

1. Copies the whole boot sector \(512 bytes\) from address 0x7c00 to 0x90000.
2. Jumps to 0x90000 and initializes the segments registers: ds, es, ss and stack pointer sp.
3. Copies the floppy parameter table from 0x78 to 0x9fef4. Patches the sector number to 18. Resets the floppy disk controller.
4. Loads the setup-sectors \(4 sectors which is 2KiB\) directly after the boot block \(0x90200\). Setup program is the next part just after the boot loader program. We are going to talk it very shortly. 
5. Gets disk drive parameters, specifically number of sectors/track, and saves it.
6. Loads the system to 0x10000. System module here refers to the the main part of the operating system, which is also running in 32-bit protected mode. Both the boot loader and the setup program are running in 16-bit real mode.
7. Sets root device number and swap device number.
8. Finishes. Jumps to the 0x90200, which is the beginning of program setup.

### Assembly syntax of bootsect.S: as86 style

If you have already read part 1 and part 2, we have already got familiar with GNU syntax of x86 assembly. Bootsect is written in another style of syntax: as86. The main difference is the positions of the source operands and the destination operands are exactly the opposite. 

### The C preprocessor: cpp

When we go through the code, we will note there is one line `#include  <linux/config.h>.` **cpp** or C preprocessor is a macro processor which is used here for transforming the source code bootsect.S to bootsect.s. During the process of the **cpp**, it copies the declarations inside of file, config.h under linux folder for this particular program, into our assembly program bootsect.S and save it as bootsect.s. 

### Files in day 1: bootsect.S, config.h and Makefile

As shown below, we examined 3 files in total. bootsect.S under boot folder is the assembly source file. Find the day 1 folder in the website for all the source files for this book. Examine each line of assembly code until we feel comfortable. There are plenty of comments in the file to help us. 

```text
day1
├── boot
│   ├── bootsect.S
│   └── Makefile
└── include
    └── linux
        └── config.h
```

The second file `config.h`which is actually a **C** head file. It defines 9 macros. At the preprocessing stage of the **C** files \(which includes `config.h`\), cpp will replace the macros with their definitions. We will also mention this at the following paragraph when we talk the **Makefile**.

It might be the first time we hear the word **Makefile** or program **make**. For now we can think **make** program is something will help us simplify the operation of compiling and debugging. Please open the file Makefile under the boot folder. The first 3 line are just macro definitions. Then we jump to line 11, this line means file bootsect.s replies on bootsect.S and ../include/linux/config.h these two file. Any of these two file changed after the last time bootsect.s was generated, **make** program will run the next line \(starts will a tab symbol\) to regenerate bootsect.s. Pretty simple, isn't it? In line 12, we see `$(CPP)`, anything insides of parentheses following a **$** is a macro, the **make** program will look up its definition and replace it, with `cpp -nostdinc -I../include` in this example. Lines 7 to 9 follow exactly the same rule. Only difference is there are two lines of commands \(line 8 and line 9 to generate file bootsect\). Finally let's look at line 5, the file name after **all:** is the target of this whole make file. So suppose you are the **make** program, when somebody key in make commands just inside of the **/day1/boot** working path, you will read the Makefile \(which is just below the boot folder\) to find the key word all. Then you understand okay the master wants to get bootsect. You keep searching the file, you figure out in order to get bootsect, you have to get bootsect.s firstly. Go on, in order to get bootsect.s we need to find bootsect.S and config.h these two files. Actually we have them, then we use the commands in line 12 to generate file bootsects. We come back line 8 and line 9 to generate file bootsect. Bootsect is actually a runnable program, a boot loader program. 

### CPP, as86, ld86 and their options

We knew as the ld in part 1 and part 2 of this book. as86 and ld86 are exactly the counterparts of as and ld. as86 assembles the source files written in as86 syntax to objective files. ld86 will link the objective files into a runnable binary file. We already briefly introduced cpp in last paragraph. For options used in this Makefile please see the comments lines in Makefile. 

That's it. See you in Day 2.

## Day 2: Collecting information from BIOS, setup.S

### Introduction to setup.S

Program setup.S reads in information from BIOS and put it in memory from 0x90000. At last of the program it switched to 32-bit protected mode. 

### Changes to /boot/Makefile

We induced some new features in the Makefile. Lines for file setup.S added as well. 

### Tree structure of our project now

We've got one more file for now. See you in day 3.

```text
day2
├── boot
│   ├── bootsect.S
│   ├── Makefile
│   └── setup.S
└── include
    └── linux
        └── config.h
```

## Day 3: Head of system module, head.s

### Main functions of program head.s

File head.s is written in AT&T style syntax of x86 assembly. It mainly covers:

1. Loads IDT and GDT.
2. Sets all the segment registers.
3. Checks A20 line.
4. Checks math chip.
5. Setups the page tables and page directory.
6. Jumps to function main. 

### Update to Makefile

Another two lines added: 

`head.o: head.s`

    `as --32 -o head.o head.s`

`head.o` added at the end the all: bootsect setup

### Files Tree

For now we have finished 3 source files under folder boot and one Makefile. 

```text
├── boot
│   ├── bootsect.S
│   ├── head.s
│   ├── Makefile
│   └── setup.S
└── include
    └── linux
        └── config.h
```

Both bootsect.S and setup.S are assembled and linked into single runnable files: bootsect and setup.

While head.o only is assembled into an objective file. It references symbols from other files. So it can only be linked at a later stage. We will get there. See you next day. 

## Day 4: Incorporate main.c

Finally we come to day 4. It will be a big day. 

A quick review what we have done in the last 3 days. We've compiled and linked two seperate programs: bootsect.S and setup.S. Then we have compiled the third file head.s to objective file but not linked it. We have to linked this object file with some other objective files or library files. Then today we are going to introduce briefly our next also very important program file main.c. A great thing is this is the first program written in C \(not in assembly\) we have encounted so far. 

The lines before the function main\(void\) includes:

C preprocessing statements like \#include, \#define etc., and variable or function declaration statements. We can temporarily ignore these and move to the code the main function. It contains:

1. Loads the swap device number, root device number and drive information into the variable . Formatting the enviroment variables and saves them. Sets size of memory and buffer.
2. Initialization before moving to user mode. Main memroy initilization. Trap, block device, character device, tty and time initilization. Scheduling, hard disk, floppy disk and buffer initiliation. The move\_to\_user\_mode\(\) macro manually creates a return enviroment for the process 0 to live.
3. Then fork\_for\_process0 firstly creates a process: process init or process 1. 
4. Process 1 never dies unless you shut down the system. It does some initilization then it goes into a big cycle. It creates a process 2, this is actually a concole program. Once this shell dies, the process 1 will create another process. 

See you in day 5. 

## Day 5: Compile main.c to main.o

Today we are going to add two makefiles and a coulple of header files.

### Main make files

To make things clearer, we put some common lines in a makefile header file. Then in the main makefile currently we only covered the lines to compile the main.c to main.o.

### Header files

As main.c referencs lots of header files, \*.h files, we need to include all these in order to compile the main.c file.

### Compile main.c

Simply key in make and press entre in command line. Make the working dictionary path the same as where the make file is.

## Day 6: Kernel.o related files

### Kernel folder

12 \*.c or \*.s files added to under kernel folder.

### Kernel make file

## Day 7: mm.o related files

### mm folder

3 \*.c or \*.s files added to under mm folder.

### mm make file

## Day 8: fs.o related files

### fs folder

18 \*.c or \*.s files added to under fs folder.

### fs make file
