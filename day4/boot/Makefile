CPP	 = cpp  -nostdinc -I../include -traditional 
AS86 = as86 -0 -a 
LD86 = ld86 -0 -s                               
# -traditional: imitate the behavior of pre-standard C preprocessors, as opposed to ISO C preprocessors.
# -a: enable partial compatibility with Minix asld. This swaps the interpretation of round brackets and square brackets as well as
#     making alterations to the code generation and syntax for 16bit jumps and calls. ("jmp @(bx)" is then a valid instruction) 
# -0: start with 16-bit code segment, warn for all instructions > 8086
# -s: remove the symbols from objective file

.S.s:
	$(CPP) -o $*.s $<
.s.o:
	$(AS86) -o $*.o $<
 
all: bootsect setup head.o

bootsect: bootsect.o
	$(LD86) -o bootsect bootsect.o
bootsect.s: bootsect.S ../include/linux/config.h

setup: setup.o
	$(LD86) -o setup setup.o
setup.s: setup.S ../include/linux/config.h

head.o: head.s
	as --32 -o head.o head.s
clean:
	rm -f bootsect bootsect.o bootsect.s	setup setup.o setup.s 	head.o

# --32: implies Intel i386 archi-tecture