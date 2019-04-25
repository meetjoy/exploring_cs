.code16                 #! tell the assembler to generate 16-bit code
.globl _start           
.section .text
_start:
# code region
    cli
    xor %ax, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    mov $_start, %sp
    sti
    cld
load_system:
    mov $0x0, %dx   # see section: Int 13/AH=02h - DISK - READ SECTOR(S) INTO MEMORY
    mov $0x2, %cx
    mov SYSSEG, %ax
    mov %ax, %es
    xor %bx, %bx
    mov $0x200 + SYSLEN, %ax
    int $0x13
    jnc ok_load
die:jmp die
ok_load:
    cli
    mov SYSSEG, %ax
    mov %ax, %ds
    xor %ax, %ax
    mov %ax, %es
    mov $0x1000, %cx
    xor %si, %si
    xor %di, %di
    rep movsb
# load idt and gdt
    xor %ds, %ds
    lidt idt_48
    lgdt gdt_48
    





# data region
SYSSEG: .word 0x1000    # 2^12 = 4KiB, the binary will be read in from here
SYSLEN: .word 17        # less than 17 sector of program head
# the below program would be superseded.
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
    cli
    call setup_ivt
    movl $system_interrupt, %eax # set IVT int 0x80
    movl %eax, 0x0200(,1) 
    movl $timer_interrupt, %eax  # set timer handler
    movl %eax, 0x0020(,1)        # timer part 1

    movb $0x36, %al     # 8293 timer setting
    movl $0x43, %edx
    outb %al, %dx
    movl $11930, %eax   # about every 1/100 second
    movl $0x40, %edx
    outb %al, %dx
    movb %ah, %al
    outb %al, %dx
    sti

    jmp .

write_char:             # set %ax, SCN_SEL,scn_pos before call               
    pushl %ebx          # put %ebx to stack
    pushw %es
    movw  SCN_SEL, %es  
    movl  scn_pos, %ebx
    cmpl  $2000, %ebx
    jb    1f
    xor   %ebx, %ebx
1:  shll  $1, %ebx      # time %bx by 2
    movw  %ax, %es:(%bx)# write 2 bytes to screen, 1 character displayed
    shrl  $1, %ebx
    addl  $1, %ebx
    movl  %ebx, %cs:scn_pos
    popw  %es
    popl  %ebx          # recover %ebx from stack
    ret

system_interrupt:      #system_interrupt handler to call write_char and iret;
    push  %ds
    pushl %edx
    pushl %ecx
    pushl %ebx
    pushl %eax
    call  write_char
    popl  %eax
    popl  %ebx
    popl  %ecx
    popl  %edx
    pop   %ds
    iret              #end of system_interrupt

timer_interrupt:
    pushl %eax
    movb  $0x20, %al
    outb  %al, $0x20
    movb  $1, %al
    cmpb  %al, %cs:current
    je    t0
    movb  $1, %cs:current
    call  task1
    jmp   t1
t0: movb  $0, %cs:current
    call  task0
t1: popl  %eax
    iret

task0:
#    pushl %eax
#    pushl %ecx
    sti
    movb $0x0A, %ah    # set the colour attributes
    movb $67, %al
    int  $0x80
    movl $0xFFF, %ecx  # pause for some time
2:  loop 2b
    jmp  task0
#    popl %ecx
#    popl %eax
    ret

task1:
    pushl %eax
    pushl %ecx
    movb $0x0D, %ah    # set the colour attributes
    movb $83, %al
    int  $0x80
    movl $0xFFF, %ecx
3:  loop 3b
    popl %ecx
    popl %eax
    ret

ignore_int:
    pushl %eax
    movb  $0x0E, %ah
    movb  $88, %al
    int   $0x80
    popl  %eax
    iret

setup_ivt:
    pushl %eax
    pushl %edx
    movl  $0, %edx
    movl  $0, %eax
    lea   ignore_int, %eax  
4:  movl  %eax, %es:(,%edx,4)
    addl  $1, %edx
    cmpl  $256, %edx
    jb    4b
    popl  %edx
    popl  %eax
    ret

jump_offset : .word main
jump_segment: .word 0x0
SCN_SEL:      .word 0xb800  #! memory number where colour text starts
scn_pos:      .long 0x0     # 2 bytes to save current screen postion
current:      .byte 0x1
.org 510                    # to advance the location to 510th byte
.word 0xAA55                # last 16 bits of first 512 bytes on disk

#as -o ep3.o ep3.s;ld ep3.o -o ep3 -Ttext=0x7c00 --oformat=binary;sudo qemu-system-x86_64 -drive format=raw,file=ep3 -cpu max