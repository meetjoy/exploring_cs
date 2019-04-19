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

# main program starts, prints an ascii code to screen
main:
    movb $0x0A, %ah     # set the colour attributes
    movb $67, %al       # set the code point to %al
    call write_char   # call function to print 0 to screen
    jmp .

# function,parametre: %ax, set %ax, SCN_SEL,scn_pos before call
write_char:           
    pushl %ebx          # put %ebx to stack
    pushw %es
    movw SCN_SEL, %es  #
    movl scn_pos, %ebx
    shll  $1, %ebx           # time %bx by 2
    movw  %ax, %es:(%bx)# write 2 bytes to screen, 1 character displayed
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

#as -o ep0.o ep0.s;ld ep0.o -o ep0 -Ttext=0x7c00 --oformat=binary;sudo qemu-system-x86_64 -drive format=raw,file=ep0 -cpu max

