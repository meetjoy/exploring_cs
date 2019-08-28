.code16                 
.globl _start           
_start:                 
    movb $0x0d, %ah     # set the colour attributes
    movb $68, %al       # set the code point to %al
    
    mov $0xb800, %cx
    movw %cx, %es  
    movl $0x0, %bx

    movw %ax, %es:(%bx) # write 2 bytes to screen, 1 character display

    jmp .

.org 510                # to advance the location to 510th byte
.word 0xAA55            # last 16 bits of first 512 bytes on disk
