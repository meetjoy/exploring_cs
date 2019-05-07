include ../../Makefile_head
CPP += -I../../include
CFLAGS += CFLAGS -finline-functions -I../../include


OBJS  = math_emulate.o error.o convert.o ea.o get_put.o \
	add.o mul.o div.o compare.o

math.a: $(OBJS)
	$(AR) rcs math.a $(OBJS)
	sync

clean:
	rm -f core *.o *.a tmp_make
	for i in *.c;do rm -f `basename $$i .c`.s;done

dep:
	sed '/\#\#\# Dependencies/q' < Makefile > tmp_make
	(for i in *.c;do echo -n `echo $$i | sed 's,\.c,\.s,'`" "; \
		$(CPP) -M $$i;done) >> tmp_make
	cp tmp_make Makefile

### Dependencies:
add.s add.o : add.c ../../include/linux/math_emu.h ../../include/linux/sched.h \
  ../../include/linux/head.h ../../include/linux/fs.h \
  ../../include/sys/types.h ../../include/linux/mm.h \
  ../../include/linux/kernel.h ../../include/signal.h 
compare.s compare.o : compare.c ../../include/linux/math_emu.h \
  ../../include/linux/sched.h ../../include/linux/head.h \
  ../../include/linux/fs.h ../../include/sys/types.h ../../include/linux/mm.h \
  ../../include/linux/kernel.h ../../include/signal.h 
convert.s convert.o : convert.c ../../include/linux/math_emu.h \
  ../../include/linux/sched.h ../../include/linux/head.h \
  ../../include/linux/fs.h ../../include/sys/types.h ../../include/linux/mm.h \
  ../../include/linux/kernel.h ../../include/signal.h 
div.s div.o : div.c ../../include/linux/math_emu.h ../../include/linux/sched.h \
  ../../include/linux/head.h ../../include/linux/fs.h \
  ../../include/sys/types.h ../../include/linux/mm.h \
  ../../include/linux/kernel.h ../../include/signal.h 
ea.s ea.o : ea.c ../../include/stddef.h ../../include/linux/math_emu.h \
  ../../include/linux/sched.h ../../include/linux/head.h \
  ../../include/linux/fs.h ../../include/sys/types.h ../../include/linux/mm.h \
  ../../include/linux/kernel.h ../../include/signal.h \
  ../../include/asm/segment.h 
error.s error.o : error.c ../../include/signal.h ../../include/sys/types.h \
  ../../include/linux/sched.h ../../include/linux/head.h \
  ../../include/linux/fs.h ../../include/linux/mm.h \
  ../../include/linux/kernel.h 
get_put.s get_put.o : get_put.c ../../include/signal.h ../../include/sys/types.h \
  ../../include/linux/math_emu.h ../../include/linux/sched.h \
  ../../include/linux/head.h ../../include/linux/fs.h \
  ../../include/linux/mm.h ../../include/linux/kernel.h \
  ../../include/asm/segment.h 
math_emulate.s math_emulate.o : math_emulate.c ../../include/signal.h \
  ../../include/sys/types.h ../../include/linux/math_emu.h \
  ../../include/linux/sched.h ../../include/linux/head.h \
  ../../include/linux/fs.h ../../include/linux/mm.h \
  ../../include/linux/kernel.h ../../include/asm/segment.h 
mul.s mul.o : mul.c ../../include/linux/math_emu.h ../../include/linux/sched.h \
  ../../include/linux/head.h ../../include/linux/fs.h \
  ../../include/sys/types.h ../../include/linux/mm.h \
  ../../include/linux/kernel.h ../../include/signal.h 
