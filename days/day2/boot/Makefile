CPP	 = cpp  -nostdinc -I../include -traditional # -traditional: imitate the behavior of pre-standard C preprocessors, as opposed to ISO C preprocessors.
AS86 = as86 -0 -a
LD86 = ld86 -0 -s                               # -s: remove the symbols from objective file

.S.s:
	$(CPP) -o $*.s $<
.s.o:
	$(AS86) -o $*.o $<
 
all: bootsect setup

bootsect: bootsect.o
	$(LD86) -o bootsect bootsect.o
bootsect.s: bootsect.S ../include/linux/config.h

setup: setup.o
	$(LD86) -o setup setup.o
setup.s: setup.S ../include/linux/config.h

clean:
	rm -f bootsect bootsect.o bootsect.s	setup setup.o setup.s 	head.o
