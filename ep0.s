.code16                 #! tell the assembler to generate 16-bit code
.globl _start           # comments start with ! indicate to be altered at a later stage
.section .text
_start:                 # main program body
SCN_SEL:  .word 0xb800  #! the segment number of video memory
scn_pos:  .word 0x0     # 2 bytes to save the current screen postion
# main program starts, prints an ascii code to screen
    mov $0x0d, %ah      # 0x0d is the colour attributes of the code to be printed
    mov $97, %al        # set the code point 97 to %al
    call write_screen   # call function to print 0 to screen
end: jmp .
# function,parametre: %ax, set %ax before call, relies on SCN_SEL,scn_pos
write_screen:           
    push %es            # put %es to stack
    pushl %ebx          # put %ebx to stack
    mov SCN_SEL, %ebx   # 
    mov %bx,%es         # set %es segments register to the video segment
    mov scn_pos, %bx    # get the value in memory of scn_pos
    mov %ax, %es:(%bx)  # write 2 bytes to screen, you will see 1 character displayed
    add $2, %bx         # add 2 to get next memory address
    cmp $4000, %bx      # compare next memory address, if > 2000 means full screen written
    jb 1f               # if < 2000, go to lable 1 
    mov $0, %bx         # if > 2000, reset next memory address to 0
1:  mov %bx, scn_pos    # save next memory address to memory
    popl %ebx           # recover %ebx from stack
    pop %es             # recover %es from stack
    ret
# end of function
.org 510                # tell the assembler to advance the location to the 510th byte
.word 0xAA55            # last two bytes of the first 512 bytes on the disk

#as -o ep0.o ep0.s;ld ep0.o -o ep0 -Ttext=0x7c00 --oformat=binary;sudo qemu-system-x86_64 -drive format=raw,file=ep0 -cpu max

