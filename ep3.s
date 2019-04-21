.code16                 #! tell the assembler to generate 16-bit code
.globl _start           # comments start with '!',to be altered at a later stage
.section .text
_start:                  
    cli                 # Turn off interrupts
    xor %ax, %ax        # Initialize the segments, set the stack to grow down from
    mov %ax, %ds        # start of bootloader at _start. SS:SP=0x0000:0x7c00
    mov %ax, %ss
    mov $_start, %sp
    mov %ax, %es
    sti                 # Enable interrupts
    cld                 # Set direction flag forward for string instructions
    ljmp *jump_offset

main: # main program starts
    #call setup_ivt
    cli
    movl $system_interrupt, %eax # set IVT int 0x80
    movl %eax, 0x0200(,1) 
    movl $timer_interrupt, %eax  # set timer handler
    movl %eax, 0x0020(,1)        # timer part 1

    movb $0x36, %al     #  8293 timer setting
    movl $0x43, %edx
    outb %al, %dx
    movl $11930, %eax   # about every 1/100 second
    movl $0x40, %edx
    outb %al, %dx
    movb %ah, %al
    outb %al, %dx

    mov  $0xE, %al      #  8259A, mask other interrupts except interrput IRQ1
    out  %al, $0x21
    mov  $0xF, %al
    out  %al, $0xA1
   
    sti
#l1: call task0
#    call task1
#    jmp l1

    jmp .

write_char:             # set %ax, SCN_SEL,scn_pos before call               
    pushl %ebx          # put %ebx to stack
    pushw %es
    movw  SCN_SEL, %es  
    movl  scn_pos, %ebx
    cmpl  $2000, %ebx
    jb    1f
    call  clear_screen
    xor   %ebx, %ebx
1:  shll  $1, %ebx      # time %bx by 2
    movw  %ax, %es:(%bx)# write 2 bytes to screen, 1 character displayed
    shrl  $1, %ebx
    addl  $1, %ebx
    movl  %ebx, scn_pos
    popw  %es
    popl  %ebx          # recover %ebx from stack
    ret
# end of function

system_interrupt:      #system_interrupt handler to call write_char and iret;
    push  %ds
    pushl %edx
    pushl %ecx
    pushl %ebx
    pushl %eax
    call write_char
    popl  %eax
    popl  %ebx
    popl  %ecx
    popl  %edx
    pop   %ds
    iret
#end of system_interrupt
timer_interrupt:                #timer part 2
    pushl %eax
    movb  $0x20, %al
    outb  %al, $0x20
    cli
    movb  $1, %al
    cmpb  %al, current
    je    st0
    movb  %al, current
    call  task1
    jmp   st1
st0:movb  $0, current
    call  task0
st1:popl  %eax
    sti
    iret

#ep3 add two functions 
task0:
    pushl %eax
    pushl %ecx
    movb $0x0A, %ah     # set the colour attributes
    movb $67, %al
    int  $0x80
    movl $0xFFFF, %ecx  # pause for some time
2:  loop 2b
 #   jmp task0
    popl %ecx
    popl %eax
    ret
task1:
    pushl %eax
    pushl %ecx
    movb $0x0D, %ah     # set the colour attributes
    movb $83, %al
    int  $0x80
    movl $0xFFFF, %ecx
3:  loop 3b
    #jmp task1
    popl %ecx
    popl %eax
    ret

#ignore_int:
#    pushl %eax
#    movb  $0x0E, %ah
#    movb  $84, %al
#   # call  write_char
#    int   $0x80
#    popl  %eax
#    iret
#
#setup_ivt:
#    pushl %eax
#    pushl %edx
#    movl  $0, %edx
#    movl  $0, %eax
#    lea   ignore_int, %eax  
#loop_ivt:  
#    movl  %eax, %es:(,%edx,4)
#   # movw  $0, %es:0x2(,%edx,4)
#    addl  $1, %edx
#    cmpl  $256, %edx
#    jb    loop_ivt
#    popl  %edx
#    popl  %eax
#    ret
#
clear_screen:
    pushl %eax
    pushl %ebx
    pushl %ecx
    pushl %edx
    mov $0x0700, %ax # function 07, AL=0 means scroll whole window
    mov $0x0007, %bh # character attribute = white on black
    mov $0x0000, %cx # row = 0, col = 0
    mov $0x184f, %dx # row = 24 (0x18), col = 79 (0x4f)
    int $0x10        # call BIOS video interrupt
    popl  %edx
    popl  %ecx
    popl  %ebx
    popl  %eax
    ret

#.section .data
jump_offset : .word main
jump_segment: .word 0x0

SCN_SEL:  .word 0xb800  #! memory number where colour text starts
scn_pos:  .word 0x0     # 2 bytes to save current screen postion
current:  .byte 0x1
.org 510                # to advance the location to 510th byte
.word 0xAA55            # last 16 bits of first 512 bytes on disk

#as -o ep3.o ep3.s;ld ep3.o -o ep3 -Ttext=0x7c00 --oformat=binary;sudo qemu-system-x86_64 -drive format=raw,file=ep3 -cpu max

