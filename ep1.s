.code16                 #! tell the assembler to generate 16-bit code
.globl _start           # comments start with '!',to be altered at a later stage
.section .text
_start:                 # main program body

# main program starts, prints the ascii code 0 to 127 to screen
    movb $0x0d, %ah      # 0x0d is the colour attributes of the code to be printed
    movb $0, %al         # set the first code point 0
1:  call write_screen    # call function to print 0 to screen
    movb %al, %bl        # temperarily saves the code point to %bl
    cmpb  $127, %al      # if %al >= 127, means all 128 ascii code all printed
    jge   end            # then jmp to end to finsh
    movb $32, %al        # else, print a space
    movw $0, %cx
2:  call write_screen
    addw $1, %cx
    cmpw $9, %cx
    jb   2b
    addb  $1, %bl        # add %bl with 1 and puts it back to %al for next round
    movb %bl, %al
    jmp 1b
end: jmp .

# function,parametre: %ax, set %ax before call, relies on SCN_SEL,scn_pos
write_screen:           
    pushw %es            # put %es to stack
    pushl %ebx           # put %ebx to stack
    movl SCN_SEL, %ebx   #
    movw %bx,%es         # set %es segments to colour text area of video RAM
    movw scn_pos, %bx    # get the value in memory of scn_pos
    cmpw $2000, %bx      # if > 2000 means full screen written
    jb 1f                # if < 2000, go to lable 1
    shrw $1, %bx
    movw $0, %bx         # if > 2000, reset next memory address to 0
1:  shlw $1, %bx
    movw %ax, %es:(%bx)  # write 2 bytes to screen, 1 character displayed
    shrw $1, %bx
    addw $1, %bx         # add 2 to get next memory address
    movw %bx, scn_pos    # save next memory address to memory
    popl %ebx            # recover %ebx from stack
    popw %es             # recover %es from stack
    ret
# end of function

SCN_SEL:  .word 0xb800  #! memory number where colour text starts
scn_pos:  .word 0x0     # 2 bytes to save current screen postion
.org 510                # to advance the location to 510th byte
.word 0xAA55            # last 16 bits of first 512 bytes on disk

#as -o ep1.o ep1.s;ld ep1.o -o ep1 -Ttext=0x7c00 --oformat=binary;sudo qemu-system-x86_64 -drive format=raw,file=ep1 -cpu max
