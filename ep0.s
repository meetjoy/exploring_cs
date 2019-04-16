.code16                 #! tell the assembler to generate 16-bit code
.globl _start           # comments start with '!',to be altered at a later stage
.section .text
_start:                 # main program body

# main program starts, prints an ascii code to screen
    movb $0x0A, %ah     # set the colour attributes
    movb $67, %al       # set the code point to %al
    call write_screen   # call function to print 0 to screen
    jmp .

# function,parametre: %ax, set %ax, SCN_SEL,scn_pos before call
write_screen:           
    pushw %es           # put %es to stack
    pushl %ebx          # put %ebx to stack
    movl  SCN_SEL, %ebx # 
    movw  %bx,%es       # set %es segment register to the colour text area of video ram
    movw  scn_pos, %bx  # get the value in memory of scn_pos
    shlw  %bx           # time %bx by 2
    movw  %ax, %es:(%bx)# write 2 bytes to screen, 1 character displayed
    popl  %ebx          # recover %ebx from stack
    popw  %es           # recover %es from stack
    ret
# end of function

SCN_SEL:  .word 0xb800  #! the segment number of video memory
scn_pos:  .word 0x0     # 2 bytes to save current screen postion
.org 510                # to advance the location to 510th byte
.word 0xAA55            # last 16 bits of first 512 bytes on disk

#as -o ep0.o ep0.s;ld ep0.o -o ep0 -Ttext=0x7c00 --oformat=binary;sudo qemu-system-x86_64 -drive format=raw,file=ep0 -cpu max

