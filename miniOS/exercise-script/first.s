.code16
.global _start
_start:
	mov $26, %cx
	mov $0x0903, %dx
	mov $0x000d, %bx
	mov $msg, %bp
	mov $0x1301, %ax
	int $0x10
loop: jmp loop
msg: .ascii "My first assembly program!"
.org 510
.word 0xAA55
