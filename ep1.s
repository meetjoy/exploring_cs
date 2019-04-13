.code16                 #! tells the assembler to generate 16-bit code
.globl _start           # comments start with ! indicate to be altered at a later stage
.section .text
_start:                 # main program body
SCN_SEL:  .word 0xb800  #! the segment number of video memory
scn_pos:  .word 0x0     # 2 bytes to save the current screen postion
.text
# prints the ascii code 32 to 126 to screen
    mov $0x0d, %ah      # 0x0d is the colour attributes of the code to be printed
    mov $0, %al         # sets the first code point 0
1:  call write_screen   # calls function to print 0 to screen
    mov %al, %bl        # temperarily saves the code point to %bl
    cmp  $127, %al      # if %al >= 127, means all 128 ascii code all printed
    jge   end           # then jmp to end to finsh
    mov $32, %al        # else, print a space
    mov $0, %cx
2:  call write_screen
    add $1, %cx
    cmp $9, %cx
    jb   2b
    add  $1, %bl        # adds %bl with 1 and puts it back to %al for next round
    mov %bl, %al
    jmp 1b
end: jmp .
# function,parametre: %ax, sets %ax before call, relies on SCN_SEL,scn_pos
write_screen:           
    push %es            # puts %es to stack
    pushl %ebx          # puts %ebx to stack
    mov SCN_SEL, %ebx   #
    mov %bx,%es         # sets %es segments register to the video segment
    mov scn_pos, %bx    # get the value in memory of scn_pos
    mov %ax, %es:(%bx)  # writes 2 bytes to screen, you will see 1 character displayed
    add $2, %bx         # adds 2 to get next memory address
    cmp $4000, %bx      # compare next memory address, if > 2000 means full screen written
    jb 1f               # if < 2000, go to lable 1 
    mov $0, %bx         # if > 2000, reset next memory address to 0
1:  mov %bx, scn_pos    # saves next memory address to memory
    popl %ebx           # recovers %ebx from stack
    pop %es             # recovers %es from stack
    ret
# end of function
.org 510                # tells the assembler to advance the location to the 510th byte
.word 0xAA55            # last two bytes of the first 512 bytes on the disk


