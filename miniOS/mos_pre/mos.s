.code16                 #! tell the assembler to generate 16-bit code
.globl _start           # comments start with '!',to be altered at a later stage
_start:                 

    movb $0x0A, %ah     # set the colour attributes
    movb $67, %al       # set the code point to %al
    xor %dx, %dx 
    movw SCN_SEL, %es  
    movl scn_pos, %ebx
    movw %ax, %es:(%bx) # write 2 bytes to screen, 1 character displaye

    jmp .

SCN_SEL:  .word 0xb800
scn_pos:  .word 0x0     # 2 bytes to save current screen postion
.org 510                # to advance the location to 510th byte
.word 0xAA55            # last 16 bits of first 512 bytes on disk

