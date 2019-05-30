// 定义了设置或修改描述符/中断门等的嵌入式汇编宏。

// 移到用户模式运行.
// 该函数利用iret指令实现从内核模式移动到初始任务0中去执行.
#define move_to_user_mode() \
__asm__ (\
	"movl %%esp, %%eax\n\t"  	/* 保存堆栈指针esp到eax寄存器中. */\
	"pushl $0x17\n\t"  			/* 首先将堆栈段选择符(SS)入栈 */\
	"pushl %%eax\n\t"  			/* 然后将保存的堆栈指针值(esp)入栈 */\
	"pushfl\n\t"  				/* 将标志寄存器(eflags)内容入栈 */\
	"pushl $0x0f\n\t"  			/* 将Task0代码段选择符(cs)入栈. */\
	"pushl $1f\n\t"  			/* 将下面标号1的偏移地址(eip)入栈. */\
	"iret\n"  					/* 执行中断返回指令,则会跳转到下面标号1处. */\
	"1:\tmovl $0x17, %%eax\n\t"  /* 此时开始执行任务0 */\
	"mov %%ax, %%ds\n\t"  		/* 初始化段寄存器指向本局部表数据段,此时ds指向内核数据段 */\
	"mov %%ax, %%es\n\t" 		/* 初始化段寄存器指向本局部表数据段,此时es指向内核数据段 */\
	"mov %%ax, %%fs\n\t" 		/* 初始化段寄存器指向本局部表数据段,此时fs指向内核数据段 */\
	"mov %%ax, %%gs" 			/* 初始化段寄存器指向本局部表数据段,此时gs指向内核数据段 */\
	:::"ax")

#define sti() __asm__ ("sti"::)										// 开中断嵌入汇编宏函数.
#define cli() __asm__ ("cli"::)										// 关中断.
#define nop() __asm__ ("nop"::)										// 空操作.

#define iret() __asm__ ("iret"::)									// 中断返回


// %0	byte 5-6	P|DPL|0 1111
// %1	gate address of lower 4 bytes
// %2	gate address of higher 4 bytes
// %3	handler offset address
// %4 	kernel selector 0x8
#define _set_gate(gate_addr, type, dpl, addr) \
__asm__ (\
	"movw %%dx, %%ax\n\t"  		/* addr to %ax */\
	"movw %0, %%dx\n\t"  		/*  */\
	"movl %%eax, %1\n\t"  		/*  */\
	"movl %%edx, %2" \
	: \
	: "i" ((short) (0x8000 + (dpl << 13) + (type << 8))), \
	"o" (*((char *) (gate_addr))), \
	"o" (*(4+(char *) (gate_addr))), \
	"d" ((char *) (addr)),"a" (0x00080000))

// 设置中断门函数(自动屏蔽随后的中断).
// 参数:n - 中断号;addr - 中断程序偏移地址.
// &idt[n]是中断描述表中中断号n对应项的偏移值;中断描述符的类型是14,特权级是0.
#define set_intr_gate(n, addr) \
	_set_gate(&idt[n], 14, 0, addr)

#define set_trap_gate(n, addr) \
	_set_gate(&idt[n], 15, 0, addr)

// 设置系统陷阱门函数.
// 上面set_trap_gate()设置的描述符的特权级为0,而这里是3.因此set_system_gate()设置的中断处理过程能够被所有程序执行.例如单步调试,溢出出错和边界超出
// 出错处理.
// 参数:n - 中断号.addr - 中断程序偏移直.
// &idt[n]是中断描述符表中中断号n对应项的偏移值;中断描述符的类型是15,特权级是3.
#define set_system_gate(n, addr) \
	_set_gate(&idt[n], 15, 3, addr)

// 设置段描述符函数(内核中没有用到).
// 参数:gate_addr - 描述符地址;type - 描述符中类型域值;dpl - 描述符特权层值;base - 段的基地址;limit - 段限长.
#define _set_seg_desc(gate_addr, type, dpl, base, limit) {\
	*((gate_addr) + 1) = ((base) & 0xff000000) | \
		(((base) & 0x00ff0000) >> 16) | \
		((limit) & 0xf0000) | \
		((dpl) << 13) | \
		(0x00408000) | \
		((type) << 8); \
	*(gate_addr) = (((base) & 0x0000ffff) << 16) | \
		((limit) & 0x0ffff); }

// 在全局表中设置任务状态段/局部表描述符.状态段局部表段的长度均被设置成104字节.
// 参数:n - 在全局表中描述符项n所对应的地址;addr - 状态段/局部表所在内存的基地址. type - 描述符中的标志类型字节.
// %0 - eax(地址addr); %1 - (描述符项n的地址); %2 - (描述符项n的地址偏移2处);
// %3 - (描述符项n的地址偏移4处); %4 - (描述符项n的地址偏移5处);%5 - (描述符项n的地址偏移6处);%6 - (描述符项n的地址偏移7处);
#define _set_tssldt_desc(n, addr, type) \
__asm__ (\
	"movw $104, %1\n\t"  									/* 将TSS(或LDT)长度放入描述符长度域(第0-1字节) */\
	"movw %%ax, %2\n\t"  									/* 将基地址的低字放入描述符第2-3字节 */\
	"rorl $16, %%eax\n\t"  									/* 将基地址高字右循环移入ax中(低字则进入高字处) */\
	"movb %%al, %3\n\t"  									/* 将基地址高中低字节移入描述符第4字节 */\
	"movb $" type ",%4\n\t"  								/* 将标志类型字节移入描述符第5字节 */\
	"movb $0x00, %5\n\t"  									/* 描述符第6字节置0 */\
	"movb %%ah, %6\n\t"  									/* 将基地址高字中高字节移入描述符第7字节 */\
	"rorl $16, %%eax"  										/* 再右循环16比特,eax恢复原值. */\
	::"a" (addr), "m" (*(n)), "m" (*(n + 2)), "m" (*(n + 4)), \
	 "m" (*(n + 5)), "m" (*(n + 6)), "m" (*(n + 7)) \
	)

// 在全局表中设置任务状态段描述符.
// n - 是该描述符的指针;addr - 是描述符项中段的基地址值.任务状态段描述符的类型是0x89
#define set_tss_desc(n, addr) _set_tssldt_desc(((char *) (n)), addr, "0x89")
// 在全局表中设置局部表描述符.
// n - 是该描述符的指针;addr - 是描述符项中段的基地址值.局部表段描述符的类型是0x82.
#define set_ldt_desc(n, addr) _set_tssldt_desc(((char *) (n)), addr, "0x82")
