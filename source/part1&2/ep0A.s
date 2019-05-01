.code16                 #! tell the assembler to generate 16-bit code
.globl _start           # comments start with '!',to be altered at a later stage
.section .text
_start:                 # 
    cli                 # Turn off interrupts
    xor %ax, %ax        # Initialize the segments, set the stack to grow down from
    mov %ax, %ds        # start of bootloader at _start. SS:SP=0x0000:0x7c00
    mov %ax, %ss
    mov $_start, %sp
    mov %ax, %es
    sti                 # Enable interrupts
    cld                 # Set direction flag forward for string instructions
    ljmp *jump_offset

# main program starts, prints the ascii code 0 to 127 to screen
main:
    movb $0x0A, %ah      # 0x0A is the colour attributes of the code to be printed
    movb $0, %al         # set the first code point 0
1:  call write_char    # call function to print 0 to screen
    movb %al, %bl        # temperarily saves the code point to %bl
    cmpb $127, %al       # if %al >= 127, means all 128 ascii code all printed
    jge   end            # then jmp to end to finsh
    movb $32, %al        # else, print a space
    xor  %ecx, %ecx
2:  call write_char
    addl $1, %ecx
    cmpl $9, %ecx
    jb   2b
    addb $1, %bl        # add %bl with 1 and puts it back to %al for next round
    movb %bl, %al
    jmp  1b
end: jmp .

# function,parametre: %ax, set %ax, SCN_SEL,scn_pos before call
write_char:           
    pushl %ebx          # put %ebx to stack
    pushw %es
    movw  SCN_SEL, %es
    movl  scn_pos, %ebx
    cmpl  $2000, %ebx   # test full screen?
    jb    1f
    xorl  %ebx, %ebx
1:  shll  $1, %ebx           # time %bx by 2
    movw  %ax, %es:(%bx)# write 2 bytes to screen, 1 character displayed
    shrl  $1, %ebx
    addl  $1, %ebx
    movl  %ebx, scn_pos
    popw  %es
    popl  %ebx          # recover %ebx from stack
    ret
# end of function

#.section .data
jump_offset : .word main
jump_segment: .word 0x0

SCN_SEL:  .word 0xb800  #! memory number where colour text starts
scn_pos:  .word 0x0     # 2 bytes to save current screen postion
.org 510                # to advance the location to 510th byte
.word 0xAA55            # last 16 bits of first 512 bytes on disk

#as -o ep0A.o ep0A.s;ld ep0A.o -o ep0A -Ttext=0x7c00 --oformat=binary;sudo qemu-system-x86_64 -drive format=raw,file=ep0A -cpu max

