include Makefile_header
ROOT_DEV = 0301
SWAP_DEV = 0304

CPP    +=  -nostdinc -Iinclude
CFLAGS +=  -nostdinc -Iinclude
LDFLAGS	+= -Ttext 0 -e startup_32

ARCHIVES=kernel/kernel.o mm/mm.o fs/fs.o
DRIVERS =kernel/blk_drv/blk_drv.a kernel/chr_drv/chr_drv.a
MATH	=kernel/math/math.a
LIBS	=lib/lib.a

.c.s:
	$(CC) $(CFLAGS) -S -o $*.s $<
.s.o:
	$(AS) -o $*.o $<
.c.o:
	$(CC) $(CFLAGS)	-c -o $*.o $<

all: Image

clean:
	rm -f init/main.s init/main.o

dep:
	sed '/\#\#\# Dependencies/q' < Makefile > tmp_make
	(for i in init/*.c;do echo -n "init/";$(CPP) -M $$i;done) >> tmp_make
	cp tmp_make Makefile
	#for i in fs kernel mm lib; do make dep -C $$i; done
	
### Dependencies:
init/main.o: init/main.c include/unistd.h include/sys/stat.h \
 include/sys/types.h include/sys/time.h include/time.h \
 include/sys/times.h include/sys/utsname.h include/sys/param.h \
 include/sys/resource.h include/utime.h include/linux/tty.h \
 include/termios.h include/linux/sched.h include/linux/head.h \
 include/linux/fs.h include/linux/mm.h include/linux/kernel.h \
 include/signal.h include/asm/system.h include/asm/io.h include/stddef.h \
 include/stdarg.h include/fcntl.h include/string.h
