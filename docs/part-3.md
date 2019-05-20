# Part 3: Start Examining An Operating System

From today, we are diving deeper to see how an operating system works. We start from 0, gradually we get an minial operating system. 

## Day 1: Boot loader program bootsect.S

The main process and functions of bootsect.S includes:

1.  Copies the whole boot sector \(512 bytes\) from address 0x7c00 to 0x90000.
2. Jumps to 0x90000 and initializes the segments registers: ds, es, ss and stack pointer sp.
3. Copies the floppy parameter table from 0x78 to 0x9fef4. Patches the sector number to 18. Resets the floppy disk controller.
4. Loads the setup-sectors \(4 sectors which is 2KiB\) directly after the bootblock \(0x90200\). Setup program is the next part just after the boot loader program. We are going to talk it very shortly. 
5. Gets disk drive parameters, specifically number of sectors/track, and saves it.
6. Loads the system to 0x10000. System module here refers to the the main part of the operating system, which is also running in 32-bit protected mode. Both the boot loader and the setup program are running in 16-bit real mode.
7. Sets root device number and swap device number.
8. Finishes. Jumps to the 0x90200, which is the beginning of program setup.

