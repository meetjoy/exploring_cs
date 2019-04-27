.globl _start           
.code16               #! tell the assembler to generate 16-bit code
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
    mov $0x7E0, %ax
    mov %ax, %ds
    xor %ax, %ax
    mov %ax, %es
    mov $0x1000, %cx
    xor %esi, %esi
    xor %edi, %edi
    rep movsw
enter_into_protect_mode:
    xor     %ax, %ax
    mov     %ax, %ds
    lidt    idt_tmp
    lgdt    gdt_tmp
    mov     $0x01, %ax
    lmsw    %ax
    ljmp    $0x08, $0x0
# data region
SYSLEN: .word 0x11      # less than 17 sector of program head
protect_code:   .word 0x0,0x0,0x08
head_offset:    .long 0x0
head_segment:   .word 0x08
idt_tmp: .word 0,0,0
gdt_tmp: .word 0x7ff, gdt_t,0
gdt_t:
        .quad   0
        .quad   0x00c09a00000007ff
        .quad   0x00c09200000007ff
.org 510
.word 0xAA55            # last 16 bits of first 512 bytes on disk

.code32
Second_Sector: 
SCN_SEL  = 0x18  #! memory number where colour text 
TSS0_SEL = 0x20
LDTO_SEL = 0x28
TSS1_SEL = 0x30
LDT1_SEL = 0x38
.text
# Establish the IDT and GDT
    movl $0x10, %eax
    mov  %ax,   %ds
    lss init_stack, %esp
    cli                 # Turn off interrupts

    lea  ignore_int, %edx   # address of default interrupt handler
    movl $0x00080000, %eax  # 16-31 bits, segment selector, 08 is the segment selector of the kernal
    mov  %dx, %ax           # 00-15 bits, offset address lower 16 bits
    movw $0x8E00, %dx       # 32-47 bits, 8 for ring 0, E for interrupt gate
                            # 48-63 bits, left blank
    lea  idt, %edi          # address of 256 off 8 bytes gates
    mov  $256, %ecx         # 
rp_idt:
    movl %eax, (%edi)
    movl %edx, 4(%edi)
    addl $8, %edi
    dec  %ecx
    jne  rp_idt
    lidt lidt_opcode        # use 6 bytes to set the idt register

    lgdt lgdt_opcode
    
    movl $0x10, %eax
    mov  %ax,   %ds
    mov  %ax,   %es
    mov  %ax,   %fs
    mov  %ax,   %gs
    lss init_stack, %esp

    movb $0x36, %al     # 8293 timer setting
    movl $0x43, %edx
    outb %al, %dx
    movl $11930, %eax   # about every 1/100 second
    movl $0x40, %edx
    outb %al, %dx
    movb %ah, %al
    outb %al, %dx
    movl $0x00080000,%eax
    mov $timer_interrupt, %ax  # set timer handler
    movl $0x8E00, %edx
    movl $0x08, %ecx
    lea  idt(,%ecx,8), %esi
    movl %eax, (%esi)
    movl %edx, 4(%esi)

    mov $system_interrupt, %ax  
    movl $0xef00, %edx
    movl $0x80, %ecx
    lea  idt(,%ecx,8), %esi
    movl %eax, (%esi)
    movl %edx, 4(%esi)
    
    # set up the return scene
    pushfl
    andl $0xffffbfff, (%esp)
    popfl
    movl $TSS0_SEL, %eax
    ltr  %ax
    movl $LDTO_SEL, %eax
    lldt %ax
    movl $0, current
    sti
    pushl   $0x17
    pushl   $init_stack
    pushfl
    pushl   $0x0f
    pushl   $task0
    iret
jmp .

write_char:             # set %ax, SCN_SEL,scn_pos before call               
    pushl %ebx          # put %ebx to stack
    pushw %es
    movw  $SCN_SEL, %bx
    movw  %bx, %es  
    movl  scn_pos, %ebx
    cmpl  $2000, %ebx
    jb    wr1
    xor   %ebx, %ebx
wr1:shll  $1, %ebx      # time %bx by 2
    movw  %ax, %es:(%bx)# write 2 bytes to screen, 1 character displayed
    shrl  $1, %ebx
    addl  $1, %ebx
    movl  %ebx, %cs:scn_pos
    popw  %es
    popl  %ebx          # recover %ebx from stack
    ret
.align 4
system_interrupt:      #system_interrupt handler to call write_char and iret;
    push  %ds
    pushl %edx
    pushl %ecx
    pushl %ebx
    pushl %eax
    movl $0x10, %edx
    mov %dx, %ds
    call  write_char
    popl  %eax
    popl  %ebx
    popl  %ecx
    popl  %edx
    pop   %ds
    iret              #end of system_interrupt
.align 4
timer_interrupt:
    push %ds
    pushl %eax
    movl $0x10, %eax
    mov %ax, %ds
    movb  $0x20, %al
    outb  %al, $0x20
    movb  $1, %al
    cmpb  %al, current
    je    t0
    movb  $1, current
    ljmp  $TSS1_SEL, $0
    jmp   t1
t0: movb  $0, current
    ljmp  $TSS0_SEL, $0
t1: 
    popl %eax
    pop  %ds
    iret

task0:
    movb $0x0A, %ah    # set the colour attributes
    movb $67, %al
    int  $0x80
    movl $0xFFF, %ecx  # pause for some time
ta2:  loop ta2
    jmp  task0

task1:
    movb $0x0D, %ah    # set the colour attributes
    movb $83, %al
    int  $0x80
    movl $0xFFF, %ecx
ta3:  loop ta3
    jmp task1

.align 4
ignore_int:
    push  %ds
    pushl %eax
    movl  $0x10, %eax
    mov   %ax, %ds
    movb  $0x0E, %ah
    movb  $88, %al
    call  write_char
    popl  %eax
    pop   %ds
    iret

# data region
scn_pos:      .long 0x0     # 2 bytes to save current screen postion
current:      .byte 0x1

.align 8
idt: .fill 256, 8, 0
lidt_opcode: .word 256*8-1
             .long idt
lgdt_opcode: .word (end_gdt-gdt-1)
             .long gdt
.align 8
gdt:    .quad   0x0000000000000000   # the first descriptor, invalid
        .quad   0x00c09a00000007ff   # kernal code segment, c-> gra size, 9->ring 0 code,a->read and execute, base =0,length = 2k*4k
        .quad   0x00c09200000007ff   # kernal data, 2->rw
        .quad   0x00c0920b80000002   # 8k
        .word   0x68, tss0, 0xe900, 0x0 # e->ring 3, 9 task gate;0x20
        .word   0x40, ldt0, 0xe200, 0x0 # ring 3, data; 0x28
        .word   0x68, tss1, 0xe900, 0x0 # 40 can be 18? to be checked
        .word   0x40, ldt1, 0xe200, 0x0 # 
end_gdt:
        .fill   128, 4, 0
init_stack:
        .long   init_stack              # kernal stack space, 512 bytes
        .word   0x10                    # kernal data segment
.align 8
ldt0:   .quad   0x0000000000000000  # invalid, 1st
        .quad   0x00c0fa00000003ff  # c:gra size; f:ring 3, code; 2^10=1k; length
        .quad   0x00c0f200000003ff  # 2:data;
tss0:   .long   0x0                 # link to last task
        .long   krn_stk0,   0x10    # ring 0, stack pointer and stack segment;
        .long   0,0,0,0,0           # esp1,ss1,esp2,ss2,cr3
        .long   0,0,0,0,0
        .long   0,0,0,0,0
        .long   0,0,0,0,0,0
        .long   LDTO_SEL, 0x8000000 #
        .fill   128, 4, 0
krn_stk0: 
.align  8
ldt1:   .quad   0x0
        .quad   0x00c0fa00000003ff  #
        .quad   0x00c0f200000003ff
tss1:   .long   0
        .long   krn_stk1, 0x10
        .long   0,0,0,0,0
        .long   task1, 0x0200
        .long   0,0,0,0
        .long   usr_stk1, 0,0,0
        .long   0x17, 0x0f, 0x17,0x17,0x17,0x17
        .long   LDT1_SEL, 0x8000000
        .fill   128, 4, 0
krn_stk1:
        .fill   128, 4, 0
usr_stk1:
