/*
 *  linux/boot/head.s
 *
 *  (C) 1991  Linus Torvalds
 */
/*
 *  head.s contains the 32-bit startup code.
 *
 * NOTE!!! Startup happens at absolute address 0x00000000, which is also where
 * the page directory will exist. The startup code will be overwritten by
 * the page directory.
 */
.text
.globl idt,gdt,pg_dir,tmp_floppy_area
pg_dir:
.globl startup_32
startup_32:
	movl $0x10, %eax					
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	lss stack_start, %esp				# stack_start defined in kernel/sched.c
	call setup_idt						
	call setup_gdt						
	movl $0x10, %eax					# reload all the segment registers
	mov %ax, %ds						# after changing gdt. CS was already
	mov %ax, %es						# reloaded in 'setup_gdt'
	mov %ax, %fs						# do we have to reload? 
	mov %ax, %gs
	lss stack_start, %esp

	xorl %eax, %eax
1:	incl %eax							# check that A20 really IS enabled
	movl %eax, 0x000000					# loop forever if it isn't
	cmpl %eax, 0x100000
	je 1b								
/*
 * NOTE! 486 should set bit 16, to check for write-protect in supervisor
 * mode. Then it would be unnecessary with the "verify_area()"-calls.
 * 486 users probably want to set the NE (#5) bit also, so as to use
 * int 16 for math errors.
 */
	movl %cr0, %eax						# check math chip
	andl $0x80000011, %eax				# Save PG,PE,ET
	/* "orl $0x10020,%eax" here for 486 might be good */
	orl $2, %eax						# set MP
	movl %eax, %cr0
	call check_x87
	jmp after_page_tables

/*
 * We depend on ET to be correct. This checks for 287/387.
 */
/*
 * 我们依赖于ET标志的正确性来检测287/387存在与否.
 *
 */
# 下面fninit和fstsw是数学协处理器(80287/80387)的指令.
# fninit向协处理器发出初始化命令,它会把协处理器置于一个末受以前操作影响的已和状态,设置其控制字为默认值,清除状态字和所有浮点栈式寄存器.非等待形式的这条指令(fninit)还
# 会让协处理器终止执行当前正在执行的任何先前的算术操作.fstsw指令取协处理器的状态字.如果系统中存在协处理器的话,那么在执行了fninit指令后其状态字低字节肯定为0.

check_x87:
	fninit								# 向协处理器发出初始化命令.
	fstsw %ax							# 取协处理器状态字到ax寄存器中.
	cmpb $0, %al						# 初始化状态字应该为0,否则说明协处理器不存在.
	je 1f								/* no coprocessor: have to set bits */
	movl %cr0, %eax						# 如果存在则向前跳转到标号1处,否则改写cr0.
	xorl $6, %eax						/* reset MP, set EM */
	movl %eax, %cr0
	ret

# 下面是一个汇编语言指示符.其含义是指存储边界对齐调整."2"表示把随后的代码或数据的偏移位置调整到地址值最后2位为零的位置(2^2),即按4字节方式对齐内存地址.不过现在GNU as直接
# 写出对齐的值而非2的次方值了.使用该指示符的目的是为了提高32位CPU访问内存中代码或数据的速度和效率.
# 下面的两个字节值是80287协处理器指令fsetpm的机器码.其作用是把80287设置为保护模式.
# 80387无需该指令,并且将会把该指令看作是空操作.

.align 2
1:	.byte 0xDB,0xE4		/* fsetpm for 287, ignored by 387 */	# 287协处理器码.
	ret

/*
 *  setup_idt
 *
 *  sets up an idt with 256 entries pointing to
 *  ignore_int, interrupt gates. It then loads
 *  idt. Everything that wants to install itself
 *  in the idt-table may do so themselves. Interrupts
 *  are enabled elsewhere, when we can be relatively
 *  sure everything is ok. This routine will be over-
 *  written by the page tables.
 */
setup_idt:
	lea ignore_int, %edx			
	movl $0x00080000, %eax			
	movw %dx, %ax					/* selector = 0x0008 = cs */	
	movw $0x8E00, %dx				/* interrupt gate - dpl=0, present */	

	lea idt, %edi					
	mov $256, %ecx
rp_sidt:
	movl %eax, (%edi)				
	movl %edx, 4(%edi)
	addl $8, %edi					
	dec %ecx
	jne rp_sidt
	lidt idt_descr					
	ret

/*
 *  setup_gdt
 *
 *  This routines sets up a new gdt and loads it.
 *  Only two entries are currently built, the same
 *  ones that were built in init.s. The routine
 *  is VERY complicated at two whole lines, so this
 *  rather long comment is certainly needed :-).
 *  This routine will be overwritten by the page tables.
 */
setup_gdt:
	lgdt gdt_descr					
	ret

/*
 * I put the kernel page tables right after the page directory,
 * using 4 of them to span 16 Mb of physical memory. People with
 * more than 16MB will have to expand this.
 */
/*
 * Linus将内核的内存页表直接放在页目录之后,使用了4个表来寻址16MB的物理内存.如果你有多于16MB的内存,就需要在这里进行扩充修改.
 *
 */
 # 每个页表长为4KB字节(1页内存页面),而每个页表项需要4个字节,因此一个页表共可以存放1024个表项.如果一个页表项寻址4KB的地址空间,则一个页表就可以寻址
 # 4MB的物理内存.
 # 页表项的格式为:项的前0-11位存放一些标志,例如是否在内存中(P位0),读写许可(R/W位1),普通还是超级用户使用(U/S位2),是否修改过了(是否脏了)(D位6)等;
 # 表项的位12-31是页框地址,用于指出一页内存的物理起始地址.

.org 0x1000							# 从偏移0x1000处开始的第1个页表(偏移0开始处将存放页表目录).
pg0:

.org 0x2000
pg1:

.org 0x3000
pg2:

.org 0x4000
pg3:

.org 0x5000							# 定义下面的内存数据块从偏移0x5000处开始.
/*
 * tmp_floppy_area is used by the floppy-driver when DMA cannot
 * reach to a buffer-block. It needs to be aligned, so that it isn't
 * on a 64kB border.
 */
/*
 * 当DMA(直接存储器访问)不能访问缓冲块时,下面的tmp_floppy_area内存块就可供软盘驱动程序使用.其地址需要对齐调整,这样就不会跨越64KB边界.
 */

tmp_floppy_area:
	.fill 1024,1,0					# 共保留1024项,每项1B,填充数值0.

 # 下面这几个入栈操作用于为跳转到init/main.c中的main()函数作准备工作.pushl $L6指令在栈中压入返回地址,而pushl $main则压入了main()函数代码
 # 的地址.当head.s最后执行ret指令时就会弹出main()的地址,并把控制权转移到init/main.c程序中.
 # 前面3个入栈0值应该分别表示envp,argv指针和argc的值,但main()没有用到.

after_page_tables:
	pushl $0						# These are the parameters to main :-)
	pushl $0						# 这些是调用main程序的参数(指init/main.c).
	pushl $0						# 其中的'$'符号表示这是一个立即操作数.
	pushl $L6						# return address for main, if it decides to.
	pushl $main						# 'main'是编译程序对main的内部表示方法.
	jmp setup_paging				# 跳转至setup_paging
L6:
	jmp L6							# main should never return here, but
									# just in case, we know what happens.
									# main程序绝对不应该返回到这里.不过为了以防万一,所以添加了该语句.这样我们就知道发生什么问题了.

/* This is the default interrupt "handler" :-) */
/* 下面是默认的中断"向量句柄" */

int_msg:
	.asciz "Unknown interrupt\n\r"	# 定义字符串"末知中断(回车换行)".

.align 2							# 按4字节方式对齐内存地址.
ignore_int:
	pushl %eax
	pushl %ecx
	pushl %edx
	push %ds						# 这里请注意!!ds,es,fs,gs等虽然是16位寄存器,但入栈后仍然会以32位的形式入栈,即需要占用4个字节的堆栈空间.
	push %es
	push %fs
	movl $0x10, %eax				# 设置段选择符(使ds,es,fs指向gdt表中的数据段).
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	pushl $int_msg					# 把调用printk函数的参数指针(地址)入栈.注意!若int_msg前不加'$',则表示把int_msg符处的长字('Unkn')入栈.
	call printk						# 该函数在/kernel/printk.c中.
	popl %eax
	pop %fs
	pop %es
	pop %ds
	popl %edx
	popl %ecx
	popl %eax
	iret							# 中断返回(把中断调用时压入栈的CPU标志寄存器(32位)值也弹出).


/*
 * Setup_paging
 *
 * This routine sets up paging by setting the page bit
 * in cr0. The page tables are set up, identity-mapping
 * the first 16MB. The pager assumes that no illegal
 * addresses are produced (ie >4Mb on a 4Mb machine).
 *
 * NOTE! Although all physical memory should be identity
 * mapped by this routine, only the kernel page functions
 * use the >1Mb addresses directly. All "normal" functions
 * use just the lower 1Mb, or the local data space, which
 * will be mapped to some other place - mm keeps track of
 * that.
 *
 * For those with more memory than 16 Mb - tough luck. I've
 * not got it, why should you :-) The source is here. Change
 * it. (Seriously - it shouldn't be too difficult. Mostly
 * change some constants etc. I left it at 16Mb, as my machine
 * even cannot be extended past that (ok, but it was cheap :-)
 * I've tried to show which constants to change by having
 * some kind of marker at them (search for "16Mb"), but I
 * won't guarantee that's all :-( )
 */
/*
 * 这个子程序通过设置控制寄存器cr0的标志(PG位31)来启动对内存的分页处理功能,并设置各个页表项的内容,以恒等映射前16MB的物理内存.分页器假定
 * 不会产生非法的地址映射(也即在只有4MB的机器上设置出大于4MB的内存地址).
 *
 * 注意!尽管所有的物理地址都应该由这个子程序进行恒等映射,但只有内核页面管理函数能直接使用>1MB的地址.所有"普通"函数仅使用低于1MB的地址空间,
 * 或者是使用局部数据空间,该地址空间将被映射到其他一些地方去--mm(内存管理程序)会管理这些事的.
 *
 */
 # 上面英文注释第2段的含义是指在机器物理内存中大于1MB的内存空间主要被用于主内存区.主内存区空间由mm模块管理.它涉及页面映射操作.内核中所有其
 # 它函数就是这里指的一般(普通)函数.若要使用主内存区的页面,就需要使用get_free_page()等函数获取.因为主内存区中内存页面是共享资源,必须有
 # 进行统一管理以避免资源争用和竞争.
 #
 # 在内存物理地址0x0处开始存放1页页目录表和4页页表.页目录表是系统所有进程共用的,而这里的4页页表则属于内核专用,它们一一映射线性地址起始16MB
 # 空间范围到物理内存上.对于新的进程,系统会在主内存区为其申请页面存放页表.另外,1页内存长度是4096字节.

.align 2								# 按4字节方式对齐内存地址边界.
setup_paging:							# 首先对5页内存(1页目录+4页页表)清零.
	movl $1024 * 5, %ecx				/* 5 pages - pg_dir+4 page tables */
	xorl %eax, %eax
	xorl %edi, %edi						/* pg_dir is at 0x000 */	# 页目录从0x0000地址开始
	cld;rep;stosl						# eax内容存到es:edi所指内存位置处,且edi增4.

	 # 下面4句设置页目录表中的项,因为我们(内核)共有4个页表所以只需设置4项.
	 # 页目录项的结构与页表项的结构一样,4个字节为1项.
	 # 例如"$pg0+7"表示:0x00001007,是页目录表中的第1项.
	 # 则第1个页表所在的地址=0x00001007 & 0xfffff000=0x1000;
	 # 第1个页表的属性标志=0x00001007 & 0x00000fff = 0x07,表示该页存在,用户可读写.
	movl $pg0 + 7, pg_dir				/* set present bit/user r/w */
	movl $pg1 + 7, pg_dir + 4			/*  --------- " " --------- */
	movl $pg2 + 7, pg_dir + 8			/*  --------- " " --------- */
	movl $pg3 + 7, pg_dir + 12			/*  --------- " " --------- */

	 # 下面6行填写4个页表中所有项的内容,共有:4(页表)*1024(项/页表)=4096项(0-0xfff),即能映射物理内存4096*4KB = 16MB.
	 # 每项的内容是:当前项所映射的物理内存地址 + 该页的标志(这里均为7).
	 # 使用的方法是从最后一个页表的最后一项开始按倒退顺序填写.一个页表的最后一项在页表中的位置是1023*4 = 4092.因此最后一页的最后一项的位置就是$pg3+4092.

	movl $pg3 + 4092, %edi				# edi->最后一页的最后一项.
	movl $0xfff007, %eax				/*  16Mb - 4096 + 7 (r/w user,p) */
										# 最后一项对应物理内存页的地址是0xfff000,加上属性标志7,即为xfff007.
	std									# 方向位置位,edi值递减(4字节).
1:	stosl								/* fill pages backwards - more efficient :-) */
	subl $0x1000, %eax					# 每填好一项,物理地址值减0x1000.
	jge 1b								# 如果小于0则说明全填写好了.
	cld
	 # 设置页目录表基地址寄存器cr3的值,指向页目录表.cr3中保存的是页目录表的物理地址.
	xorl %eax, %eax						/* pg_dir is at 0x0000 */		# 页目录表在0x0000处.
	movl %eax, %cr3						/* cr3 - page directory start */
	 # 设置启动使用分页处理(cr0的PG标志,位31)
	movl %cr0, %eax
	orl $0x80000000, %eax				# 添上PG标志.
	movl %eax, %cr0						/* set paging (PG) bit */
	ret									/* this also flushes prefetch-queue */

# 在改变分页处理标志后要求使用转移指令刷新预取指令队列, 这里用的是返回指令ret.
# 该返回指令的另一个作用是将pushl $main压入堆栈中的main程序的地址弹出,并跳转到/init/main.c程序去运行.本程序到此就真正结束了.

.align 2								# 按4字节方式对齐内存地址边界.
.word 0									# 这里先空出2字节,这样.long _idt的长字是4字节对齐的.

 # 下面是加载中断描述符表寄存器idtr的指令lidt要求的6字节操作数.前2字节是idt表的限长,后4字节是idt表在线性地址空间中的32位基地址.
idt_descr:
	.word 256 * 8 - 1					# idt contains 256 entries
	.long idt
.align 2
.word 0

 # 下面是加载全局描述符表寄存器gdtr的指令lgdt要求的6字节操作数.前2字节是gdt表的限长,后4字节是gdt表的线性基地址.这里全局表长度设置为
 # 2KB字节(0x7ff即可),因为每8字节组成一个描述符项,所以表中共可有256项.符号gdt是全局表在本程序中的偏移位置.

gdt_descr:
	.word 256 * 8 - 1					# so does gdt (not that that's any
	.long gdt							# magic number, but it works for me :^)

	.align 8							# 按8(2^3)字节方式对齐内存地址边界.
idt:	.fill 256, 8, 0					# idt is uninitialized	# 256项,每项8字节,填0.

 # (0-nul, 1-cs, 2-ds, 3-syscall, 4-TSS0, 5-LDT0, 6-TSS1, 7-LDT1, 8-TSS2 etc...)
gdt:
	.quad 0x0000000000000000			/* NULL descriptor */
	.quad 0x00c09a0000000fff			/* 16Mb */		
	.quad 0x00c0920000000fff			/* 16Mb */		
	.quad 0x0000000000000000			/* TEMPORARY - don't use */
	.fill 252, 8, 0						/* space for LDT's and TSS's etc */	
