.code16               #! tell the assembler to generate 16-bit code
.globl _start           
.section .text
_start:
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
load_to_RAM_from_2nd_Sector:
    mov $0x00, %edx   # see section: Int 13/AH=02h - DISK - READ SECTOR(S) INTO MEMORY
    mov $0x02, %ecx
    mov $0x7E0, %ax   # Read in the 2nd sector from disk to memory 0x7E00
    mov %ax, %es        
    xor %bx, %bx
    movb $0x02, %ah
    movb SYSLEN, %al
    int $0x13
    jnc ok_load
die:jmp die

ok_load:
    cli
    mov $0x7E0, %ax
    mov %ax, %ds
    xor %ax, %ax
    mov %ax, %es
    mov $0x1000, %cx
    xor %esi, %esi
    xor %edi, %edi
    rep movsw
    ljmp *head_offset

# data region
SYSLEN: .word 0x11      # less than 17 sector of program head
head_offset: .word 0x7E00
head_segment:.word 0x0
.org 510
.word 0xAA55            # last 16 bits of first 512 bytes on disk

Second_Sector: 
    cli                 # Turn off interrupts
    xor %ax, %ax        # Initialize the segments, set the stack to grow down from
    mov %ax, %ds        # start of bootloader at _start. SS:SP=0x0000:0x7c00
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    mov $0x7C00, %sp
    cld                 # Set direction flag forward for string instructions
main: # main program starts
#    call setup_ivt
    pushl %eax
    pushl %edx
    pushw %es
    movl  $0, %edx
    movl  $0, %eax
    movw  %ax, %es
    lea   ignore_int, %ax
loop_ivt:  
    movl  %eax, %es:(,%edx,4)
    addl  $1, %edx
    cmpl  $256, %edx
    jb    loop_ivt
    popw  %es
    popl  %edx
    popl  %eax
#jmp .
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
    jb    wr1
    xor   %ebx, %ebx
wr1:  shll  $1, %ebx      # time %bx by 2
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
    movb  $0x20, %al
    outb  %al, $0x20
    movb  $1, %al
    cmpb  %al, %cs:current
    je    t0
    movb  $1, %cs:current
    jmp  task1
    jmp   t1
t0: movb  $0, %cs:current
    jmp  task0
t1: 
    iret

task0:
    sti
    movb $0x0A, %ah    # set the colour attributes
    movb $67, %al
    int  $0x80
    movl $0xFFF, %ecx  # pause for some time
ta2:  loop ta2
    jmp  task0

task1:
    sti
    movb $0x0D, %ah    # set the colour attributes
    movb $83, %al
    int  $0x80
    movl $0xFFF, %ecx
ta3:  loop ta3
    jmp task1

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
    pushw %es
    movl  $0, %edx
    movl  $0, %eax
    movw  %ax, %es
    lea   ignore_int, %ax
loop_set_ivt:  
    movl  %eax, %es:(,%edx,4)
    addl  $1, %edx
    cmpl  $256, %edx
    jb    loop_set_ivt
    popw  %es
    popl  %edx
    popl  %eax
    ret

SCN_SEL:      .word 0xb800  #! memory number where colour text starts
scn_pos:      .long 0x0     # 2 bytes to save current screen postion
current:      .byte 0x1

# as boot0.s -o boot0.o;ld -o boot0 boot0.o  -Ttext=0x7c00 --oformat=binary;sudo qemu-system-x86_64 -drive format=raw,file=boot0,index=0,if=floppy -cpu max
