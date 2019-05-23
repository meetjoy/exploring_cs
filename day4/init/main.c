/*
 *  linux/init/main.c
 *  (C) 1991  Linus Torvalds
 */
/*
 * we need this inline - forking from kernel space will result
 * in NO COPY ON WRITE (!!!), until an execve is executed. This
 * is no problem, but for the stack. This is handled by not letting
 * main() use the stack at all after fork(). Thus, no function
 * calls - which means inline code for fork too, as otherwise we
 * would use the stack upon exit from 'fork()'.
 *
 * Actually only pause and fork are needed inline, so that there
 * won't be any messing with the stack from main(), but we define
 * some others too.
 */

// __attribute__((always_inline))
// int fork(void) __attribute__((always_inline));
// int pause(void) __attribute__((always_inline));

#ifndef _EM
#define _EM
#endif

#define __LIBRARY__
#include <unistd.h>
#include <time.h>

_syscall0(int, fork)
_syscall0(int, pause)
_syscall1(int, setup, void *, BIOS)
_syscall0(int, sync)

#include <linux/tty.h>                  			
#include <linux/sched.h>							
#include <asm/system.h>								
#include <asm/io.h>									

#include <stddef.h>                     			
#include <stdarg.h>									
													
// #include <unistd.h>
#include <fcntl.h>                      			
//#include <linux/head.h>
//#include <sys/types.h>

#include <linux/fs.h>								
													
#include <linux/kernel.h>							

#include <string.h>									

static char printbuf[1024];							

extern char *strcpy();
extern int vsprintf();								
extern void init(void);								
extern void blk_dev_init(void);						
extern void chr_dev_init(void);						
extern void hd_init(void);							
extern void floppy_init(void);						
extern void mem_init(long start, long end);			
extern long rd_init(long mem_start, int length);	
extern long kernel_mktime(struct tm * tm);			

static inline long fork_for_process0() {
	long __res;
	__asm__ volatile (
		"int $0x80\n\t"  														
		: "=a" (__res)  														
		: "0" (2));  															
	if (__res >= 0)  															
		return __res;
	errno = -__res;  															
	return -1;
}

static int sprintf(char * str, const char *fmt, ...)
{
	va_list args;
	int i;

	va_start(args, fmt);
	i = vsprintf(str, fmt, args);
	va_end(args);
	return i;
}

/*
 * This is set up by the setup-routine at boot-time
 */
#define EXT_MEM_K (*(unsigned short *)0x90002)							
#define CON_ROWS ((*(unsigned short *)0x9000e) & 0xff)					
#define CON_COLS (((*(unsigned short *)0x9000e) & 0xff00) >> 8)
#define DRIVE_INFO (*((struct drive_info *)0x90080))					
#define ORIG_ROOT_DEV (*(unsigned short *)0x901FC)						
#define ORIG_SWAP_DEV (*(unsigned short *)0x901FA)						

/*
 * Yeah, yeah, it's ugly, but I cannot find how to do this correctly
 * and this seems to work. I anybody has more info on the real-time
 * clock I'd be interested. Most of this was trial and error, and some
 * bios-listing reading. Urghh.
 */
#define CMOS_READ(addr) ({ \
	outb_p(0x80 | addr, 0x70); 					\
	inb_p(0x71); 								\
})

#define BCD_TO_BIN(val) ((val) = ((val)&15) + ((val) >> 4) * 10)

static void time_init(void)
{
	struct tm time;								
	do {
		time.tm_sec = CMOS_READ(0);				
		time.tm_min = CMOS_READ(2);				
		time.tm_hour = CMOS_READ(4);			
		time.tm_mday = CMOS_READ(7);			
		time.tm_mon = CMOS_READ(8);				
		time.tm_year = CMOS_READ(9);			
	} while (time.tm_sec != CMOS_READ(0));
	BCD_TO_BIN(time.tm_sec);					
	BCD_TO_BIN(time.tm_min);
	BCD_TO_BIN(time.tm_hour);
	BCD_TO_BIN(time.tm_mday);
	BCD_TO_BIN(time.tm_mon);
	BCD_TO_BIN(time.tm_year);
	time.tm_mon--;								
	startup_time = kernel_mktime(&time);		
}

static long memory_end = 0;						
static long buffer_memory_end = 0;				
static long main_memory_start = 0;				
static char term[32];							

static char * argv_rc[] = { "/bin/sh", NULL };		
static char * envp_rc[] = { "HOME=/", NULL ,NULL };	

static char * argv[] = { "-/bin/sh",NULL };			
													
static char * envp[] = { "HOME=/usr/root", NULL, NULL };

struct drive_info { char dummy[32]; } drive_info;	

int main(void)	/* This really IS void, no error here. */
{				/* The startup routine assumes (well, ...) this */
#ifdef _EM
	__asm__("movl %cr0,%eax \n\t" \
	        "xorl $6,%eax \n\t" \
	        "movl %eax,%cr0");
#endif
	/*
	 * Interrupts are still disabled. Do necessary setups, then
	 * enable them
	 */
	// 首先保存根文件系统设备和交换文件设备号,并根据setup.s程序中获取的信息设置控制台终端屏幕行,列数环境变量TERM,并用其设置初始init进程
	// 中执行etc/rc文件和shell程序使用的环境变量,以及复制内存0x90080处的硬盘表.
	// 其中ROOT_DEV已在前面包含进的include/linux/fs.h文件上被声明为extern_int
	// 而SWAP_DEV在include/linux/mm.h文件内也作了相同声明.这里mm.h文件并没有显式地列在本程序前部,因为前面包含进的include/linux/sched.h
	// 文件中已经含有它.
 	ROOT_DEV = ORIG_ROOT_DEV;										// ROOT_DEV定义在fs/super.c
 	SWAP_DEV = ORIG_SWAP_DEV;										// SWAP_DEV定义在mm/swap.c
   	sprintf(term, "TERM=con%dx%d", CON_COLS, CON_ROWS);
	envp[1] = term;
	envp_rc[1] = term;
    drive_info = DRIVE_INFO;										// 复制内存0x90080处的硬盘参数表.

	// 接着根据机器物理内存容量设置高速缓冲区和主内存的位置和范围.
	// 高速缓存末端地址->buffer_memory_end;机器内存容量->memory_end;主内存开始地址->main_memory_start.
	// 设置物理内存大小
	memory_end = (1 << 20) + (EXT_MEM_K << 10);						// 内存大小=1MB + 扩展内存(k)*1024字节.
	memory_end &= 0xfffff000;										// 忽略不到4KB(1页)的内存数.
	if (memory_end > 16 * 1024 * 1024)								// 如果内存量超过16MB,则按16MB计.
		memory_end = 16 * 1024 * 1024;
	// 根据物理内存的大小设置高速缓冲去的末端大小
	if (memory_end > 12 * 1024 * 1024) 								// 如果内存>12MB,则设置缓冲区末端=4MB
		buffer_memory_end = 4 * 1024 * 1024;
	else if (memory_end > 6 * 1024 * 1024)							// 否则若内存>6MB,则设置缓冲区末端=2MB
		buffer_memory_end = 2 * 1024 * 1024;
	else
		buffer_memory_end = 1 * 1024 * 1024;						// 否则则设置缓冲区末端=1MB
	// 根据高速缓冲区的末端大小设置主内存区的起始地址
	main_memory_start = buffer_memory_end;							// 主内存起始位置 = 缓冲区末端
	// 如果在Makefile文件中定义了内存虚拟盘符号RAMDISK,则初始化虚拟盘.此时主优点将减少.
	// 参见kernel/blk_drv/ramdisk.c.
#ifdef RAMDISK
	main_memory_start += rd_init(main_memory_start, RAMDISK * 1024);
#endif
	// 以下是内核进行所有方面的初始化工作.
	mem_init(main_memory_start, memory_end);						// 主内存区初始化.(mm/memory.c)
	trap_init();                                    				// 陷阱门(硬件中断向量)初始化.(kernel/traps.c)
	blk_dev_init();													// 块设备初始化.(blk_drv/ll_rw_blk.c)
	chr_dev_init();													// 字符设备初始化.(chr_drv/tty_io.c)
 	tty_init();														// tty初始化(chr_drv/tty_io.c)
	time_init();													// 设置开机启动时间.
 	sched_init();													// 调度程序初始化(加载任务0的tr,ldtr)(kernel/sched.c)
	buffer_init(buffer_memory_end);									// 缓冲管理初始化,建内存链表等.(fs/buffer.c)
	hd_init();														// 硬盘初始化.	(blk_drv/hd.c)
	floppy_init();													// 软驱初始化.	(blk_drv/floppy.c)
	sti();															// 所有初始化工作都完了,于是开启中断.
	// 打印内核初始化完毕
	Log(LOG_INFO_TYPE, "<<<<< Linux0.12 Kernel Init Finished, Ready Start Process0 >>>>>\n");
	// 下面过程通过在堆栈中设置的参数,利用中断返回指令启动任务0执行.
	move_to_user_mode();											// 移到用户模式下执行.(include/asm/system.h)
	if (!fork_for_process0()) {										/* we count on this going ok */
		init();														// 在新建的子进程(任务1即init进程)中执行.
	}
	/*
	 *   NOTE!!   For any other task 'pause()' would mean we have to get a
	 * signal to awaken, but task0 is the sole exception (see 'schedule()')
	 * as task 0 gets activated at every idle moment (when no other tasks
	 * can run). For task0 'pause()' just means we go check if some other
	 * task can run, and if not we return here.
	 */
	/*
	 * 注意!!对于任何其他任务,'pause()'将意味着我们必须等待收到一个信号才会返回就绪态,但任务0(task0)是唯一例外情况(参见'schedule()'),因为
	 * 任务0在任何空闲时间里都会被激活(当没有其他任务在运行时),因此对于任务0'pause()'仅意味着我们返回来查看是否有其他任务可以运行,如果没有的话
	 * 我们就回到这里,一直循环执行'pause()'.
	 */
	// pause()系统调用(kernel/sched.c)会把任务0转换成可中断等待状态,再执行调度函数.但是调度函数只要发现系统中没有其他任务可以运行时就会切换
	// 到任务0,是不信赖于任务0的状态.
	for(;;)
		__asm__("int $0x80"::"a" (__NR_pause):);					// 即执行系统调用pause().
}

// 下面函数产生格式化信息并输出到标准输出设备stdout(1),这里是指屏幕上显示.参数'*fmt'指定输出将采用的格式,参见标准C语言书籍.
// 该子程序正好是vsprintf如何使用的一个简单例子.该程序使用vsprintf()将格式化的字符串放入printbuf缓冲区,然后用write()将
// 缓冲区的内容输出到标准设备(1--stdout).vsprintf()函数的实现见kernel/vsprintf.c.
int printf(const char *fmt, ...)
{
	va_list args;
	int i;

	va_start(args, fmt);
	write(1, printbuf, i = vsprintf(printbuf, fmt, args));
	va_end(args);
	return i;
}

// 在main()中已经进行子系统初始化,包括内存管理,各种硬件设备和驱动程序.init()函数在任务0第1次创建的子进程(任务1)中.它首先对第一个将要执行
// 的程序(shell)的环境进行初始化,然后以登录shell方式加载程序并执行之.
void init(void)
{
	int pid, i, fd;
	// setup()是一个系统调用.用于读取硬盘参数和分区表信息并加载虚拟盘(若存在的话)和安装根文件系统设备.该函数用25行上的宏定义,对就函数是sys_setup(),
	// 在块设备子目录kernel/blk_drv/hd.c.
	setup((void *) &drive_info);
	// 下面以读写访问方式打开设备"/dev/tty0",它对应终端控制台.由于这是第一次打开文件操作,因此产生的文件句柄号(文件描述符)肯定是0.该句柄是UNIX类操作
	// 系统默认的控制台标准输入句柄stdin.这里再把它以读和写的方式分别打开是为了复制产生标准输出(写)句柄stdout和标准出错输出句柄stderr.函数前面的"(void)"
	// 前缀用于表示强制函数无需返回值.
	(void) open("/dev/tty1", O_RDWR, 0);
	(void) dup(0);													// 复制句柄,产生句柄1号--stdout标准输出设备.
	(void) dup(0);													// 复制句柄,产生句柄2号--stderr标准出错输出设备.
	// 进程1执行的用户级代码的开始
	printf("<<<<< Process 1 console fd = %d >>>>>\n", fd);
	// 下面打印缓冲区块数和总字节数,每块1024字节,以及主内存区空闲内存字节数.
	printf("<<<<< %d buffers = %d bytes buffer space >>>>>\n\r", NR_BUFFERS,
			NR_BUFFERS * BLOCK_SIZE);
	printf("<<<<< Free mem: %d bytes >>>>>\n\r", memory_end - main_memory_start);
	// 下面fork()用于创建一个子进程(任务2).对于被创建的子进程,fork()将返回0值,对于原进程(父进程)则返回子进程的进程号pid.所以第202--206行是子进程执行的内容.
	// 该子进程关闭了句柄0(stdin),以只读方式打开/etc/rc文件,并使用execve()函数将进程自身替换成/bin/sh程序(即shell程序),然后执行/bin/sh程序.所携带的参数
	// 和环境变量分别由argv_rc和envp_rc数组给出.关闭句柄0并立刻打开/etc/rc文件的作用是把标准输入stdin重定向到/etc/rc/文件.这样shell程序/bin/sh就可以运行
	// rc文件中设置的命令.由于这里sh的运行方式是非交互式的,因此在执行完rc文件中的命令后就会立刻退出,进程2也随之结束.并于execve()函数说明请参见fs/exec.c程序.
	// 函数_exit()退出时的出错码1 - 操作未许可;2 -- 文件或目录不存在.
	if (!(pid = fork())) {
		close(0);
		if (open("/etc/rc", O_RDONLY, 0))
			_exit(1);												// 若打开文件失败,则退出(lib/_exit.c).
		execve("/bin/sh", argv_rc, envp_rc);						// 替换成/bin/sh程序并执行.
		_exit(2);													// 若execve()执行失败则退出.
    }
	// 下面还是父进程（1）执行的语句。wait()等待子进程停止或终止，返回值应是子进程的进程号（pid)。这三句的作用是父进程等待子进程
	// 的结束。&i是存放返回状态信息的位置。如果wait()返回值不等于子进程号，则继续等待。
  	if (pid > 0)
		while (pid != wait(&i));
	// 如果执行到这里，说明刚创建的子进程的执行已停止或终止了。下面循环中首先再创建一个子进程，如果出错，则显示“初始化程序创建子进程
	// 失败”信息并继续执行。对于所创建的子进程将关闭所有以前还遗留的句柄（stdin、stdout、stderr），新创建一个会话并设置进程组号，
	// 然后重新打开/dev/tty0作为stdin，并复制成stdout和stderr。再次执行系统解释程序/bin/sh。但这次执行所选用的参数和环境数组另
	// 选了一套。然后父进程再次运行wait()等等。如果子进程又停止了执行，则在标准输出上显示出错信息“子进程pid停止了运行，返回码是i”，
	// 然后继续重试下去...，形成“大”死循环。
	while (1) {
		if ((pid = fork()) < 0) {
			printf("Fork failed in init %c\r\n", ' ');
			continue;
		}
		if (!pid) {                             					// 新的子进程。
			close(0); close(1); close(2);
			setsid();                       						// 创建一新的会话期，见后面说明。
			(void) open("/dev/tty1", O_RDWR, 0);
			(void) dup(0);
			(void) dup(0);
			_exit(execve("/bin/sh", argv, envp));
		}
		while (1)
			if (pid == wait(&i))
				break;
		printf("\n\rchild %d died with code %04x\n\r", pid, i);
		sync();
	}
	_exit(0);														/* NOTE! _exit, not exit() */
}


