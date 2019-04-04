    .code16
    .global _start
_start:
    mov $48, %cx
    mov $0x0F01, %dx
    mov $0x0001, %bx
    mov $msg, %bp
    mov $0x1301, %ax
    int $0x10
    jmp .
msg:.ascii "Hello, World! This is my first assembly program."
    .org 510
    .word 0xAA55

#as first.s -o first.o
#ld -Ttext=0x7c00 -o first.img --oformat=binary first.o
#sudo qemu-system-i386 -drive format=raw,file=first.img




