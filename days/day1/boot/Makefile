CPP	 = cpp  -nostdinc -I../include
AS86 = as86 -0 -a
LD86 = ld86 -0 -s

all: bootsect

bootsect: bootsect.s
	$(AS86) -o bootsect.o bootsect.s
	$(LD86) -o bootsect   bootsect.o  

bootsect.s: bootsect.S ../include/linux/config.h
	$(CPP) -traditional bootsect.S -o bootsect.s 
# -s: remove the symbols from objective file
# -traditional: imitate the behavior of pre-standard C preprocessors, as opposed to ISO C preprocessors.

clean:
	rm -f bootsect bootsect.o bootsect.s setup setup.o setup.s head.o
