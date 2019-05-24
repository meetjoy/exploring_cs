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
	// we can try to change the return type to void see what happens
#ifdef _EM
	__asm__("movl %cr0,%eax \n\t" \
	        "xorl $6,%eax \n\t" \
	        "movl %eax,%cr0");
	// bit 2, EM, If set, no x87 floating-point unit present
	// If the EM bit is set, all FPU and vector operations will cause a #UD so they can be EMulated in software. 
	// bit 1, MP	Monitor co-processor, Controls interaction of WAIT/FWAIT instructions with TS flag in CR0
	// This does little else other than saying if an FWAIT opcode is exempted from responding to the TS bit. 
	// Since FWAIT will force serialisation of exceptions, it should normally be set to the inverse of the EM bit, 
	// so that FWAIT will actually cause a FPU state update when FPU instructions are asynchronous, and not when they are emulated. 
#endif
	/*
	 * Interrupts are still disabled. Do necessary setups, then enable them
	 */
 	ROOT_DEV = ORIG_ROOT_DEV;										// ROOT_DEV in fs/super.c
 	SWAP_DEV = ORIG_SWAP_DEV;										// SWAP_DEV in mm/swap.c
   	sprintf(term, "TERM=con%dx%d", CON_COLS, CON_ROWS);
	envp[1] = term;
	envp_rc[1] = term;
    drive_info = DRIVE_INFO;										// copy hard disk table from 0x90080

	memory_end = (1 << 20) + (EXT_MEM_K << 10);						// 1 MiB + EXT_MEM_K * 1024
	memory_end &= 0xfffff000;										// Ignore the tail part less than 2^12 = 4k
	if (memory_end > 16 * 1024 * 1024)								//
		memory_end = 16 * 1024 * 1024;
	if (memory_end > 12 * 1024 * 1024) 								//
		buffer_memory_end = 4 * 1024 * 1024;
	else if (memory_end > 6 * 1024 * 1024)							//
		buffer_memory_end = 2 * 1024 * 1024;
	else
		buffer_memory_end = 1 * 1024 * 1024;						//
	main_memory_start = buffer_memory_end;							// 
	// kernel/blk_drv/ramdisk.c.
#ifdef RAMDISK
	main_memory_start += rd_init(main_memory_start, RAMDISK * 1024);
#endif
	mem_init(main_memory_start, memory_end);						// (mm/memory.c)
	trap_init();                                    				// (kernel/traps.c)
	blk_dev_init();													// (blk_drv/ll_rw_blk.c)
	chr_dev_init();													// (chr_drv/tty_io.c)
 	tty_init();														// tty init (chr_drv/tty_io.c)
	time_init();													// set startup time
 	sched_init();													// (load task 0: tr,ldtr)(kernel/sched.c)
	buffer_init(buffer_memory_end);									// (fs/buffer.c)
	hd_init();														// (blk_drv/hd.c)
	floppy_init();													// (blk_drv/floppy.c)
	sti();															// 
	Log(LOG_INFO_TYPE, "<<<<< Linux-0.12 Kernel init finished, ready to start process 0 >>>>>\n");
	move_to_user_mode();											// (include/asm/system.h)
	if (!fork_for_process0()) {										/* we count on this going ok */
		init();														// 
	}
	/*
	 *   NOTE!!   For any other task 'pause()' would mean we have to get a
	 * signal to awaken, but task0 is the sole exception (see 'schedule()')
	 * as task 0 gets activated at every idle moment (when no other tasks
	 * can run). For task0 'pause()' just means we go check if some other
	 * task can run, and if not we return here.
	 */
	for(;;)
		__asm__("int $0x80"::"a" (__NR_pause):);					
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


