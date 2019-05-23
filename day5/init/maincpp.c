# 1 "main.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "main.c"
# 27 "main.c"
# 1 "../include/unistd.h" 1
# 55 "../include/unistd.h"
# 1 "../include/sys/stat.h" 1



# 1 "../include/sys/types.h" 1





typedef unsigned int size_t;




typedef long time_t;




typedef long ptrdiff_t;






typedef int pid_t;
typedef unsigned short uid_t;
typedef unsigned short gid_t;
typedef unsigned short dev_t;
typedef unsigned short ino_t;
typedef unsigned short mode_t;
typedef unsigned short umode_t;
typedef unsigned char nlink_t;
typedef int daddr_t;
typedef long off_t;
typedef unsigned char u_char;
typedef unsigned short ushort;

typedef unsigned char cc_t;
typedef unsigned int speed_t;
typedef unsigned long tcflag_t;

typedef unsigned long fd_set;

typedef struct { int quot,rem; } div_t;
typedef struct { long quot,rem; } ldiv_t;


struct ustat {
 daddr_t f_tfree;
 ino_t f_tinode;
 char f_fname[6];
 char f_fpack[6];
};
# 5 "../include/sys/stat.h" 2

struct stat {
 dev_t st_dev;
 ino_t st_ino;
 umode_t st_mode;
 nlink_t st_nlink;
 uid_t st_uid;
 gid_t st_gid;
 dev_t st_rdev;
 off_t st_size;
 time_t st_atime;
 time_t st_mtime;
 time_t st_ctime;
};
# 60 "../include/sys/stat.h"
extern int chmod(const char *_path, mode_t mode);
extern int fstat(int fildes, struct stat *stat_buf);
extern int mkdir(const char *_path, mode_t mode);
extern int mkfifo(const char *_path, mode_t mode);
extern int stat(const char *filename, struct stat *stat_buf);
extern mode_t umask(mode_t mask);
# 56 "../include/unistd.h" 2
# 1 "../include/sys/time.h" 1




struct timeval {
 long tv_sec;
 long tv_usec;
};


struct timezone {
 int tz_minuteswest;
 int tz_dsttime;
};
# 56 "../include/sys/time.h"
struct itimerval {
 struct timeval it_interval;
 struct timeval it_value;
};

# 1 "../include/time.h" 1
# 20 "../include/time.h"
typedef long clock_t;

struct tm {
 int tm_sec;
 int tm_min;
 int tm_hour;
 int tm_mday;
 int tm_mon;
 int tm_year;
 int tm_wday;
 int tm_yday;
 int tm_isdst;
};




clock_t clock(void);
time_t time(time_t * tp);
double difftime(time_t time2, time_t time1);
time_t mktime(struct tm * tp);

char * asctime(const struct tm * tp);
char * ctime(const time_t * tp);
struct tm * gmtime(const time_t *tp);
struct tm *localtime(const time_t * tp);
size_t strftime(char * s, size_t smax, const char * fmt, const struct tm * tp);
void tzset(void);
# 62 "../include/sys/time.h" 2


int gettimeofday(struct timeval * tp, struct timezone * tz);
# 57 "../include/unistd.h" 2
# 1 "../include/sys/times.h" 1





struct tms {
 time_t tms_utime;
 time_t tms_stime;
 time_t tms_cutime;
 time_t tms_cstime;
};

extern time_t times(struct tms * tp);
# 58 "../include/unistd.h" 2
# 1 "../include/sys/utsname.h" 1




# 1 "../include/sys/param.h" 1
# 6 "../include/sys/utsname.h" 2

struct utsname {
 char sysname[9];
 char nodename[8 +1];
 char release[9];
 char version[9];
 char machine[9];
};

extern int uname(struct utsname * utsbuf);
# 59 "../include/unistd.h" 2
# 1 "../include/sys/resource.h" 1
# 32 "../include/sys/resource.h"
struct rusage {
 struct timeval ru_utime;
 struct timeval ru_stime;
 long ru_maxrss;
 long ru_ixrss;
 long ru_idrss;
 long ru_isrss;
 long ru_minflt;
 long ru_majflt;
 long ru_nswap;
 long ru_inblock;
 long ru_oublock;
 long ru_msgsnd;
 long ru_msgrcv;
 long ru_nsignals;
 long ru_nvcsw;
 long ru_nivcsw;
};
# 80 "../include/sys/resource.h"
struct rlimit {
 int rlim_cur;
 int rlim_max;
};
# 60 "../include/unistd.h" 2
# 1 "../include/utime.h" 1






struct utimbuf {
 time_t actime;
 time_t modtime;
};


extern int utime(const char *filename, struct utimbuf *times);
# 61 "../include/unistd.h" 2
# 210 "../include/unistd.h"
extern int errno;

int access(const char * filename, mode_t mode);
int acct(const char * filename);
int alarm(int sec);
int brk(void * end_data_segment);

int chdir(const char * filename);
int chmod(const char * filename, mode_t mode);
int chown(const char * filename, uid_t owner, gid_t group);
int chroot(const char * filename);
int close(int fildes);
int creat(const char * filename, mode_t mode);
int dup(int fildes);
int execve(const char * filename, char ** argv, char ** envp);





void exit(int status);
void _exit(int status);
int fcntl(int fildes, int cmd, ...);
int fork(void);
int getpid(void);
int getuid(void);
int geteuid(void);
int getgid(void);
int getegid(void);
int ioctl(int fildes, int cmd, ...);
int kill(pid_t pid, int signal);
int link(const char * filename1, const char * filename2);
int lseek(int fildes, off_t offset, int origin);
int mknod(const char * filename, mode_t mode, dev_t dev);
int mount(const char * specialfile, const char * dir, int rwflag);
int nice(int val);
int open(const char * filename, int flag, ...);
int pause(void);
int pipe(int * fildes);
int read(int fildes, char * buf, off_t count);

int setpgid(pid_t pid,pid_t pgid);
int setuid(uid_t uid);
int setgid(gid_t gid);
void (*signal(int sig, void (*fn)(int)))(int);
int stat(const char * filename, struct stat * stat_buf);
int fstat(int fildes, struct stat * stat_buf);
int stime(time_t * tptr);
int sync(void);
time_t time(time_t * tloc);
time_t times(struct tms * tbuf);
int ulimit(int cmd, long limit);
mode_t umask(mode_t mask);
int umount(const char * specialfile);
int uname(struct utsname * name);
int unlink(const char * filename);
int ustat(dev_t dev, struct ustat * ubuf);
int utime(const char * filename, struct utimbuf * times);
pid_t waitpid(pid_t pid,int * wait_stat,int options);
pid_t wait(int * wait_stat);
int write(int fildes, const char * buf, off_t count);
int dup2(int oldfd, int newfd);
int getppid(void);
pid_t getpgrp(void);
pid_t setsid(void);
int sethostname(char *name, int len);
int setrlimit(int resource, struct rlimit *rlp);
int getrlimit(int resource, struct rlimit *rlp);
int getrusage(int who, struct rusage *rusage);
int gettimeofday(struct timeval *tv, struct timezone *tz);
int settimeofday(struct timeval *tv, struct timezone *tz);
int getgroups(int gidsetlen, gid_t *gidset);
int setgroups(int gidsetlen, gid_t *gidset);
int select(int width, fd_set * readfds, fd_set * writefds,
 fd_set * exceptfds, struct timeval * timeout);
# 28 "main.c" 2


int fork(void) { long __res; __asm__ volatile ("int $0x80" : "=a" (__res) : "0" (2));if (__res >= 0) return (int) __res; errno = -__res;return -1; }
int pause(void) { long __res; __asm__ volatile ("int $0x80" : "=a" (__res) : "0" (29));if (__res >= 0) return (int) __res; errno = -__res;return -1; }
int setup(void * BIOS) { long __res; __asm__ volatile ("int $0x80" : "=a" (__res) : "0" (0), "b" ((long)(BIOS)));if (__res >= 0) return (int) __res;errno = -__res; return -1; }
int sync(void) { long __res; __asm__ volatile ("int $0x80" : "=a" (__res) : "0" (36));if (__res >= 0) return (int) __res; errno = -__res;return -1; }

# 1 "../include/linux/tty.h" 1
# 16 "../include/linux/tty.h"
extern int NR_CONSOLES;

# 1 "../include/termios.h" 1
# 82 "../include/termios.h"
struct winsize {
 unsigned short ws_row;
 unsigned short ws_col;
 unsigned short ws_xpixel;
 unsigned short ws_ypixel;
};



struct termio {
 unsigned short c_iflag;
 unsigned short c_oflag;
 unsigned short c_cflag;
 unsigned short c_lflag;
 unsigned char c_line;
 unsigned char c_cc[8];
};



struct termios {
 tcflag_t c_iflag;
 tcflag_t c_oflag;
 tcflag_t c_cflag;
 tcflag_t c_lflag;
 cc_t c_line;
 cc_t c_cc[17];
};
# 269 "../include/termios.h"
extern speed_t cfgetispeed(struct termios *termios_p);

extern speed_t cfgetospeed(struct termios *termios_p);

extern int cfsetispeed(struct termios *termios_p, speed_t speed);

extern int cfsetospeed(struct termios *termios_p, speed_t speed);

extern int tcdrain(int fildes);

extern int tcflow(int fildes, int action);

extern int tcflush(int fildes, int queue_selector);

extern int tcgetattr(int fildes, struct termios *termios_p);

extern int tcsendbreak(int fildes, int duration);

extern int tcsetattr(int fildes, int optional_actions,
 struct termios *termios_p);
# 19 "../include/linux/tty.h" 2



struct tty_queue {
 unsigned long data;
 unsigned long head;
 unsigned long tail;
 struct task_struct * proc_list;
 char buf[1024];
};
# 56 "../include/linux/tty.h"
struct tty_struct {
 struct termios termios;
 int pgrp;
 int session;
 int stopped;
 void (*write)(struct tty_struct * tty);
 struct tty_queue *read_q;
 struct tty_queue *write_q;
 struct tty_queue *secondary;
 };

extern struct tty_struct tty_table[];
extern int fg_console;
# 81 "../include/linux/tty.h"
void rs_init(void);
void con_init(void);
void tty_init(void);

int tty_read(unsigned c, char * buf, int n);
int tty_write(unsigned c, char * buf, int n);

void con_write(struct tty_struct * tty);
void rs_write(struct tty_struct * tty);
void mpty_write(struct tty_struct * tty);
void spty_write(struct tty_struct * tty);

void copy_to_cooked(struct tty_struct * tty);

void update_screen(void);
# 36 "main.c" 2
# 1 "../include/linux/sched.h" 1
# 34 "../include/linux/sched.h"
# 1 "../include/linux/head.h" 1



typedef struct desc_struct {
 unsigned long a,b;
} desc_table[256];

extern unsigned long pg_dir[1024];
extern desc_table idt,gdt;
# 35 "../include/linux/sched.h" 2
# 1 "../include/linux/fs.h" 1
# 31 "../include/linux/fs.h"
void buffer_init(long buffer_end);
# 71 "../include/linux/fs.h"
typedef char buffer_block[1024];

struct buffer_head {
 char * b_data;
 unsigned long b_blocknr;
 unsigned short b_dev;
 unsigned char b_uptodate;
 unsigned char b_dirt;
 unsigned char b_count;
 unsigned char b_lock;
 struct task_struct * b_wait;
 struct buffer_head * b_prev;
 struct buffer_head * b_next;
 struct buffer_head * b_prev_free;
 struct buffer_head * b_next_free;
};

struct d_inode {
 unsigned short i_mode;
 unsigned short i_uid;
 unsigned long i_size;
 unsigned long i_time;
 unsigned char i_gid;
 unsigned char i_nlinks;
 unsigned short i_zone[9];

};

struct m_inode {
 unsigned short i_mode;
 unsigned short i_uid;
 unsigned long i_size;
 unsigned long i_mtime;
 unsigned char i_gid;
 unsigned char i_nlinks;
 unsigned short i_zone[9];

 struct task_struct * i_wait;
 struct task_struct * i_wait2;
 unsigned long i_atime;
 unsigned long i_ctime;
 unsigned short i_dev;
 unsigned short i_num;
 unsigned short i_count;
 unsigned char i_lock;
 unsigned char i_dirt;
 unsigned char i_pipe;
 unsigned char i_mount;
 unsigned char i_seek;
 unsigned char i_update;
};

struct file {
 unsigned short f_mode;
 unsigned short f_flags;
 unsigned short f_count;
 struct m_inode * f_inode;
 off_t f_pos;
};

struct super_block {
 unsigned short s_ninodes;
 unsigned short s_nzones;
 unsigned short s_imap_blocks;
 unsigned short s_zmap_blocks;
 unsigned short s_firstdatazone;
 unsigned short s_log_zone_size;
 unsigned long s_max_size;
 unsigned short s_magic;

 struct buffer_head * s_imap[8];
 struct buffer_head * s_zmap[8];
 unsigned short s_dev;
 struct m_inode * s_isup;
 struct m_inode * s_imount;
 unsigned long s_time;
 struct task_struct * s_wait;
 unsigned char s_lock;
 unsigned char s_rd_only;
 unsigned char s_dirt;
};

struct d_super_block {
 unsigned short s_ninodes;
 unsigned short s_nzones;
 unsigned short s_imap_blocks;
 unsigned short s_zmap_blocks;
 unsigned short s_firstdatazone;
 unsigned short s_log_zone_size;
 unsigned long s_max_size;
 unsigned short s_magic;
};

struct dir_entry {
 unsigned short inode;
 char name[14];
};

extern struct m_inode inode_table[64];
extern struct file file_table[64];
extern struct super_block super_block[8];
extern struct buffer_head * start_buffer;
extern int nr_buffers;

extern void check_disk_change(int dev);
extern int floppy_change(unsigned int nr);
extern int ticks_to_floppy_on(unsigned int dev);
extern void floppy_on(unsigned int dev);
extern void floppy_off(unsigned int dev);
extern void truncate(struct m_inode * inode);
extern void sync_inodes(void);
extern void wait_on(struct m_inode * inode);
extern int bmap(struct m_inode * inode,int block);
extern int create_block(struct m_inode * inode,int block);

extern struct m_inode * namei(const char * pathname);
extern struct m_inode * lnamei(const char * pathname);
extern int open_namei(const char * pathname, int flag, int mode,
 struct m_inode ** res_inode);
extern void iput(struct m_inode * inode);
extern struct m_inode * iget(int dev,int nr);
extern struct m_inode * get_empty_inode(void);
extern struct m_inode * get_pipe_inode(void);
extern struct buffer_head * get_hash_table(int dev, int block);
extern struct buffer_head * getblk(int dev, int block);
extern void ll_rw_block(int rw, struct buffer_head * bh);
extern void ll_rw_page(int rw, int dev, int nr, char * buffer);
extern void brelse(struct buffer_head * buf);
extern struct buffer_head * bread(int dev,int block);
extern void bread_page(unsigned long addr,int dev,int b[4]);
extern struct buffer_head * breada(int dev,int block,...);
extern int new_block(int dev);
extern int free_block(int dev, int block);
extern struct m_inode * new_inode(int dev);
extern void free_inode(struct m_inode * inode);
extern int sync_dev(int dev);
extern struct super_block * get_super(int dev);
extern int ROOT_DEV;
extern void put_super(int dev);
extern void invalidate_inodes(int dev);

extern void mount_root(void);
# 36 "../include/linux/sched.h" 2
# 1 "../include/linux/mm.h" 1






# 1 "../include/linux/kernel.h" 1




void verify_area(void * addr,int count);
void panic(const char * str);
void do_exit(long error_code);
int printf(const char * fmt, ...);
int printk(const char * fmt, ...);
void console_print(const char * str);
int tty_write(unsigned ch,char * buf,int count);
void * malloc(unsigned int size);
void free_s(void * obj, int size);
extern void hd_times_out(void);
extern void sysbeepstop(void);
extern void hd_times_out(void);
extern void sysbeepstop(void);
extern void blank_screen(void);
extern void unblank_screen(void);
extern int beepcount;
extern int hd_timeout;
extern int blankinterval;
extern int blankcount;





extern void Log(unsigned short log_level, const char *fmt, ...);
# 8 "../include/linux/mm.h" 2
# 1 "../include/signal.h" 1





typedef int sig_atomic_t;
typedef unsigned int sigset_t;
# 68 "../include/signal.h"
struct sigaction {
 void (*sa_handler)(int);
 sigset_t sa_mask;
 int sa_flags;
 void (*sa_restorer)(void);
};




void (*signal(int _sig, void (*_func)(int)))(int);


int raise(int sig);
int kill(pid_t pid, int sig);
# 91 "../include/signal.h"
int sigaddset(sigset_t *mask, int signo);
int sigdelset(sigset_t *mask, int signo);
int sigemptyset(sigset_t *mask);
int sigfillset(sigset_t *mask);
int sigismember(sigset_t *mask, int signo);

int sigpending(sigset_t *set);


int sigprocmask(int how, sigset_t *set, sigset_t *oldset);


int sigsuspend(sigset_t *sigmask);

int sigaction(int sig, struct sigaction *act, struct sigaction *oldact);
# 9 "../include/linux/mm.h" 2

extern int SWAP_DEV;






extern unsigned long get_free_page(void);
extern unsigned long put_dirty_page(unsigned long page,unsigned long address);
extern void free_page(unsigned long addr);
extern void init_swapping(void);
void swap_free(int page_nr);
void swap_in(unsigned long *table_ptr);



static inline void oom(void)
{
 printk("out of memory\n\r");

 do_exit(11);
}
# 43 "../include/linux/mm.h"
extern unsigned long HIGH_MEMORY;







extern unsigned char mem_map [ ((15 * 1024 * 1024) >> 12) ];
# 37 "../include/linux/sched.h" 2
# 57 "../include/linux/sched.h"
extern int copy_page_tables(unsigned long from, unsigned long to, long size);

extern int free_page_tables(unsigned long from, unsigned long size);


extern void sched_init(void);

extern void schedule(void);

extern void trap_init(void);

extern void panic(const char * str);

extern int tty_write(unsigned minor,char * buf,int count);

typedef int (*fn_ptr)();

struct i387_struct {
 long cwd;
 long swd;
 long twd;
 long fip;
 long fcs;
 long foo;
 long fos;
 long st_space[20];
};

struct tss_struct {
 long back_link;
 long esp0;
 long ss0;
 long esp1;
 long ss1;
 long esp2;
 long ss2;
 long cr3;
 long eip;
 long eflags;
 long eax;
 long ecx;
 long edx;
 long ebx;
 long esp;
 long ebp;
 long esi;
 long edi;
 long es;
 long cs;
 long ss;
 long ds;
 long fs;
 long gs;
 long ldt;
 long trace_bitmap;
 struct i387_struct i387;
};
# 163 "../include/linux/sched.h"
struct task_struct {

 long state;
 long counter;
 long priority;
 long signal;
 struct sigaction sigaction[32];
 long blocked;


 int exit_code;
 unsigned long start_code;
 unsigned long end_code;
 unsigned long end_data;
 unsigned long brk;
 unsigned long start_stack;
 long pid;
 long pgrp;
 long session;
 long leader;
 int groups[32];





 struct task_struct *p_pptr;
 struct task_struct *p_cptr;
 struct task_struct *p_ysptr;
 struct task_struct *p_osptr;
 unsigned short uid;
 unsigned short euid;
 unsigned short suid;
 unsigned short gid;
 unsigned short egid;
 unsigned short sgid;
 unsigned long timeout;
 unsigned long alarm;
 long utime;
 long stime;
 long cutime;
 long cstime;
 long start_time;
 struct rlimit rlim[6];

 unsigned int flags;
 unsigned short used_math;



 int tty;
 unsigned short umask;
 struct m_inode * pwd;
 struct m_inode * root;
 struct m_inode * executable;
 struct m_inode * library;
 unsigned long close_on_exec;
 struct file * filp[20];

 struct desc_struct ldt[3];

 struct tss_struct tss;
};
# 268 "../include/linux/sched.h"
extern struct task_struct *task[64];
extern struct task_struct *last_task_used_math;
extern struct task_struct *current;

extern unsigned long volatile jiffies;
extern unsigned long startup_time;
extern int jiffies_offset;



extern void add_timer(long jiffies, void (*fn)(void));
extern void sleep_on(struct task_struct ** p);
extern void interruptible_sleep_on(struct task_struct ** p);
extern void wake_up(struct task_struct ** p);
extern int in_group_p(gid_t grp);
# 37 "main.c" 2
# 1 "../include/asm/system.h" 1
# 38 "main.c" 2
# 1 "../include/asm/io.h" 1
# 39 "main.c" 2

# 1 "../include/stddef.h" 1
# 41 "main.c" 2
# 1 "../include/stdarg.h" 1




typedef char *va_list;
# 22 "../include/stdarg.h"
void va_end (va_list);
# 42 "main.c" 2


# 1 "../include/fcntl.h" 1
# 44 "../include/fcntl.h"
struct flock {
 short l_type;
 short l_whence;
 off_t l_start;
 off_t l_len;
 pid_t l_pid;
};

extern int creat(const char * filename,mode_t mode);



extern int fcntl(int fildes,int cmd, ...);
extern int open(const char * filename, int flags, ...);
# 45 "main.c" 2





# 1 "../include/linux/kernel.h" 1




void verify_area(void * addr,int count);
void panic(const char * str);
void do_exit(long error_code);
int printf(const char * fmt, ...);
int printk(const char * fmt, ...);
void console_print(const char * str);
int tty_write(unsigned ch,char * buf,int count);
void * malloc(unsigned int size);
void free_s(void * obj, int size);
extern void hd_times_out(void);
extern void sysbeepstop(void);
extern void hd_times_out(void);
extern void sysbeepstop(void);
extern void blank_screen(void);
extern void unblank_screen(void);
extern int beepcount;
extern int hd_timeout;
extern int blankinterval;
extern int blankcount;





extern void Log(unsigned short log_level, const char *fmt, ...);
# 51 "main.c" 2

# 1 "../include/string.h" 1
# 13 "../include/string.h"
extern char * strerror(int errno);
# 28 "../include/string.h"
static inline char * strcpy(char * dest, const char *src)
{
__asm__(
 "cld\n"
 "1:\tlodsb\n\t"
 "stosb\n\t"
 "testb %%al, %%al\n\t"
 "jne 1b"
 ::"S" (src), "D" (dest):"ax");
return dest;
}


static inline char * strncpy(char * dest, const char *src, int count)
{
__asm__(
 "cld\n"
 "1:\tdecl %2\n\t"
 "js 2f\n\t"
 "lodsb\n\t"
 "stosb\n\t"
 "testb %%al, %%al\n\t"
 "jne 1b\n\t"
 "rep\n\t"
 "stosb\n"
 "2:"
 ::"S" (src), "D" (dest), "c" (count):"ax");
return dest;
}


static inline char * strcat(char * dest, const char * src)
{
__asm__(
 "cld\n\t"
 "repne\n\t"
 "scasb\n\t"
 "decl %1\n"
 "1:\tlodsb\n\t"
 "stosb\n\t"
 "testb %%al, %%al\n\t"
 "jne 1b"
 ::"S" (src), "D" (dest), "a" (0), "c" (0xffffffff):);
return dest;
}


static inline char * strncat(char * dest, const char * src, int count)
{
__asm__(
 "cld\n\t"
 "repne\n\t"
 "scasb\n\t"
 "decl %1\n\t"
 "movl %4, %3\n"
 "1:\tdecl %3\n\t"
 "js 2f\n\t"
 "lodsb\n\t"
 "stosb\n\t"
 "testb %%al, %%al\n\t"
 "jne 1b\n"
 "2:\txorl %2, %2\n\t"
 "stosb"
 ::"S" (src), "D" (dest), "a" (0), "c" (0xffffffff), "g" (count)
 :);
return dest;
}

static inline int strcmp(const char * cs, const char * ct)
{
register int __res __asm__("ax");
__asm__(
 "cld\n"
 "1:\tlodsb\n\t"
 "scasb\n\t"
 "jne 2f\n\t"
 "testb %%al, %%al\n\t"
 "jne 1b\n\t"
 "xorl %%eax, %%eax\n\t"
 "jmp 3f\n"
 "2:\tmovl $1, %%eax\n\t"
 "jl 3f\n\t"
 "negl %%eax\n"
 "3:"
 :"=a" (__res):"D" (cs), "S" (ct):);
return __res;
}

static inline int strncmp(const char * cs, const char * ct, int count)
{
register int __res __asm__("ax");
__asm__(
 "cld\n"
 "1:\tdecl %3\n\t"
 "js 2f\n\t"
 "lodsb\n\t"
 "scasb\n\t"
 "jne 3f\n\t"
 "testb %%al, %%al\n\t"
 "jne 1b\n"
 "2:\txorl %%eax, %%eax\n\t"
 "jmp 4f\n"
 "3:\tmovl $1, %%eax\n\t"
 "jl 4f\n\t"
 "negl %%eax\n"
 "4:"
 :"=a" (__res):"D" (cs), "S" (ct), "c" (count):);
return __res;
}

static inline char * strchr(const char * s, char c)
{
register char * __res __asm__("ax");
__asm__(
 "cld\n\t"
 "movb %%al, %%ah\n"
 "1:\tlodsb\n\t"
 "cmpb %%ah, %%al\n\t"
 "je 2f\n\t"
 "testb %%al, %%al\n\t"
 "jne 1b\n\t"
 "movl $1, %1\n"
 "2:\tmovl %1, %0\n\t"
 "decl %0"
 :"=a" (__res):"S" (s), "0" (c):);
return __res;
}

static inline char * strrchr(const char * s, char c)
{
register char * __res __asm__("dx");
__asm__(
 "cld\n\t"
 "movb %%al, %%ah\n"
 "1:\tlodsb\n\t"
 "cmpb %%ah, %%al\n\t"
 "jne 2f\n\t"
 "movl %%esi, %0\n\t"
 "decl %0\n"
 "2:\ttestb %%al, %%al\n\t"
 "jne 1b"
 :"=d" (__res):"0" (0), "S" (s), "a" (c):);
return __res;
}

static inline int strspn(const char * cs, const char * ct)
{
register char * __res __asm__("si");
__asm__(
 "cld\n\t"
 "movl %4, %%edi\n\t"
 "repne\n\t"
 "scasb\n\t"
 "notl %%ecx\n\t"
 "decl %%ecx\n\t"
 "movl %%ecx, %%edx\n"
 "1:\tlodsb\n\t"
 "testb %%al, %%al\n\t"
 "je 2f\n\t"
 "movl %4, %%edi\n\t"
 "movl %%edx, %%ecx\n\t"
 "repne\n\t"
 "scasb\n\t"
 "je 1b\n"
 "2:\tdecl %0"
 :"=S" (__res):"a" (0), "c" (0xffffffff), "0" (cs), "g" (ct)
 :"dx","di");
return __res - cs;
}

static inline int strcspn(const char * cs, const char * ct)
{
register char * __res __asm__("si");
__asm__(
 "cld\n\t"
 "movl %4, %%edi\n\t"
 "repne\n\t"
 "scasb\n\t"
 "notl %%ecx\n\t"
 "decl %%ecx\n\t"
 "movl %%ecx, %%edx\n"
 "1:\tlodsb\n\t"
 "testb %%al, %%al\n\t"
 "je 2f\n\t"
 "movl %4, %%edi\n\t"
 "movl %%edx, %%ecx\n\t"
 "repne\n\t"
 "scasb\n\t"
 "jne 1b\n"
 "2:\tdecl %0"
 :"=S" (__res):"a" (0), "c" (0xffffffff), "0" (cs), "g" (ct)
 :"dx", "di");
return __res - cs;
}

static inline char * strpbrk(const char * cs, const char * ct)
{
register char * __res __asm__("si");
__asm__(
 "cld\n\t"
 "movl %4, %%edi\n\t"
 "repne\n\t"
 "scasb\n\t"
 "notl %%ecx\n\t"
 "decl %%ecx\n\t"
 "movl %%ecx, %%edx\n"
 "1:\tlodsb\n\t"
 "testb %%al, %%al\n\t"
 "je 2f\n\t"
 "movl %4, %%edi\n\t"
 "movl %%edx, %%ecx\n\t"
 "repne\n\t"
 "scasb\n\t"
 "jne 1b\n\t"
 "decl %0\n\t"
 "jmp 3f\n"
 "2:\txorl %0, %0\n"
 "3:"
 :"=S" (__res):"a" (0), "c" (0xffffffff), "0" (cs), "g" (ct)
 :"dx", "di");
return __res;
}

static inline char * strstr(const char * cs,const char * ct)
{
register char * __res __asm__("ax");
__asm__(
 "cld\n\t"
 "movl %4, %%edi\n\t"
 "repne\n\t"
 "scasb\n\t"
 "notl %%ecx\n\t"
 "decl %%ecx\n\t"

 "movl %%ecx, %%edx\n"
 "1:\tmovl %4, %%edi\n\t"
 "movl %%esi, %%eax\n\t"
 "movl %%edx, %%ecx\n\t"
 "repe\n\t"
 "cmpsb\n\t"
 "je 2f\n\t"

 "xchgl %%eax, %%esi\n\t"
 "incl %%esi\n\t"
 "cmpb $0, -1(%%eax)\n\t"
 "jne 1b\n\t"
 "xorl %%eax, %%eax\n\t"
 "2:"
 :"=a" (__res):"0" (0), "c" (0xffffffff), "S" (cs), "g" (ct)
 :"dx", "di");
return __res;
}

static inline int strlen(const char * s)
{
register int __res __asm__("cx");
__asm__(
 "cld\n\t"
 "repne\n\t"
 "scasb\n\t"
 "notl %0\n\t"
 "decl %0"
 :"=c" (__res):"D" (s), "a" (0), "0" (0xffffffff));
return __res;
}

char * ___strtok;

static inline char * strtok(char * s, const char * ct)
{
register char * __res;
__asm__("testl %1, %1\n\t"
 "jne 1f\n\t"
 "testl %0,%0\n\t"
 "je 8f\n\t"
 "movl %0, %1\n"
 "1:\txorl %0, %0\n\t"
 "movl $-1, %%ecx\n\t"
 "xorl %%eax, %%eax\n\t"
 "cld\n\t"
 "movl %4, %%edi\n\t"
 "repne\n\t"
 "scasb\n\t"
 "notl %%ecx\n\t"
 "decl %%ecx\n\t"
 "je 7f\n\t"

 "movl %%ecx, %%edx\n"
 "2:\tlodsb\n\t"
 "testb %%al, %%al\n\t"
 "je 7f\n\t"
 "movl %4, %%edi\n\t"
 "movl %%edx, %%ecx\n\t"
 "repne\n\t"
 "scasb\n\t"
 "je 2b\n\t"
 "decl %1\n\t"
 "cmpb $0, (%1)\n\t"
 "je 7f\n\t"
 "movl %1,%0\n"
 "3:\tlodsb\n\t"
 "testb %%al, %%al\n\t"
 "je 5f\n\t"
 "movl %4, %%edi\n\t"
 "movl %%edx, %%ecx\n\t"
 "repne\n\t"
 "scasb\n\t"
 "jne 3b\n\t"
 "decl %1\n\t"
 "cmpb $0, (%1)\n\t"
 "je 5f\n\t"
 "movb $0, (%1)\n\t"
 "incl %1\n\t"
 "jmp 6f\n"
 "5:\txorl %1, %1\n"
 "6:\tcmpb $0, (%0)\n\t"
 "jne 7f\n\t"
 "xorl %0, %0\n"
 "7:\ttestl %0, %0\n\t"
 "jne 8f\n\t"
 "movl %0, %1\n"
 "8:"
 :"=b" (__res), "=S" (___strtok)
 :"0" (___strtok), "1" (s), "g" (ct)
 :"ax", "cx", "dx", "di");
return __res;
}

static inline void * memcpy(void * dest, const void * src, int n)
{
__asm__(
 "cld\n\t"
 "rep\n\t"
 "movsb"
 ::"c" (n), "S" (src), "D" (dest)
 :);
return dest;
}

static inline void * memmove(void * dest, const void * src, int n)
{
if (dest < src){
__asm__(
 "cld\n\t"
 "rep\n\t"
 "movsb"
 ::"c" (n), "S" (src), "D" (dest)
 :);
} else {
__asm__(
 "std\n\t"
 "rep\n\t"
 "movsb\n\t"
    "cld"
 ::"c" (n), "S" (src + n - 1), "D" (dest + n - 1)
 :);
}
return dest;
}

static inline int memcmp(const void * cs, const void * ct, int count)
{
register int __res __asm__("ax");
__asm__(
 "cld\n\t"
 "repe\n\t"
 "cmpsb\n\t"
 "je 1f\n\t"
 "movl $1, %%eax\n\t"
 "jl 1f\n\t"
 "negl %%eax\n"
 "1:"
 :"=a" (__res):"0" (0), "D" (cs), "S" (ct), "c" (count)
 :);
return __res;
}

static inline void * memchr(const void * cs, char c, int count)
{
register void * __res;
if (!count)
 return ((void *)0);
__asm__(
 "cld\n\t"
 "repne\n\t"
 "scasb\n\t"
 "je 1f\n\t"
 "movl $1, %0\n"
 "1:\tdecl %0"
 :"=D" (__res):"a" (c), "D" (cs), "c" (count)
 :);
return __res;
}

static inline void * memset(void * s, char c, int count)
{
__asm__(
  "cld\n\t"
  "pushl %%edi\n\t"
  "rep\n\t"
  "stosb\n\t"
  "popl %%edi"
  ::"a" (c), "D" (s), "c" (count)
  :);
return s;
}
# 53 "main.c" 2

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

 (args = ((char *) &(fmt) + (((sizeof (fmt) + sizeof (int) - 1) / sizeof (int)) * sizeof (int))));
 i = vsprintf(str, fmt, args);
 ;
 return i;
}
# 113 "main.c"
static void time_init(void)
{
 struct tm time;
 do {
  time.tm_sec = ({ __asm__ ("outb %%al,%%dx\n" "\tjmp 1f\n" "1:\tjmp 1f\n" "1:"::"a" (0x80 | 0),"d" (0x70)); ({ unsigned char _v; __asm__ volatile ("inb %%dx,%%al\n" "\tjmp 1f\n" "1:\tjmp 1f\n" "1:":"=a" (_v):"d" (0x71)); _v; }); });
  time.tm_min = ({ __asm__ ("outb %%al,%%dx\n" "\tjmp 1f\n" "1:\tjmp 1f\n" "1:"::"a" (0x80 | 2),"d" (0x70)); ({ unsigned char _v; __asm__ volatile ("inb %%dx,%%al\n" "\tjmp 1f\n" "1:\tjmp 1f\n" "1:":"=a" (_v):"d" (0x71)); _v; }); });
  time.tm_hour = ({ __asm__ ("outb %%al,%%dx\n" "\tjmp 1f\n" "1:\tjmp 1f\n" "1:"::"a" (0x80 | 4),"d" (0x70)); ({ unsigned char _v; __asm__ volatile ("inb %%dx,%%al\n" "\tjmp 1f\n" "1:\tjmp 1f\n" "1:":"=a" (_v):"d" (0x71)); _v; }); });
  time.tm_mday = ({ __asm__ ("outb %%al,%%dx\n" "\tjmp 1f\n" "1:\tjmp 1f\n" "1:"::"a" (0x80 | 7),"d" (0x70)); ({ unsigned char _v; __asm__ volatile ("inb %%dx,%%al\n" "\tjmp 1f\n" "1:\tjmp 1f\n" "1:":"=a" (_v):"d" (0x71)); _v; }); });
  time.tm_mon = ({ __asm__ ("outb %%al,%%dx\n" "\tjmp 1f\n" "1:\tjmp 1f\n" "1:"::"a" (0x80 | 8),"d" (0x70)); ({ unsigned char _v; __asm__ volatile ("inb %%dx,%%al\n" "\tjmp 1f\n" "1:\tjmp 1f\n" "1:":"=a" (_v):"d" (0x71)); _v; }); });
  time.tm_year = ({ __asm__ ("outb %%al,%%dx\n" "\tjmp 1f\n" "1:\tjmp 1f\n" "1:"::"a" (0x80 | 9),"d" (0x70)); ({ unsigned char _v; __asm__ volatile ("inb %%dx,%%al\n" "\tjmp 1f\n" "1:\tjmp 1f\n" "1:":"=a" (_v):"d" (0x71)); _v; }); });
 } while (time.tm_sec != ({ __asm__ ("outb %%al,%%dx\n" "\tjmp 1f\n" "1:\tjmp 1f\n" "1:"::"a" (0x80 | 0),"d" (0x70)); ({ unsigned char _v; __asm__ volatile ("inb %%dx,%%al\n" "\tjmp 1f\n" "1:\tjmp 1f\n" "1:":"=a" (_v):"d" (0x71)); _v; }); }));
 ((time.tm_sec) = ((time.tm_sec)&15) + ((time.tm_sec) >> 4) * 10);
 ((time.tm_min) = ((time.tm_min)&15) + ((time.tm_min) >> 4) * 10);
 ((time.tm_hour) = ((time.tm_hour)&15) + ((time.tm_hour) >> 4) * 10);
 ((time.tm_mday) = ((time.tm_mday)&15) + ((time.tm_mday) >> 4) * 10);
 ((time.tm_mon) = ((time.tm_mon)&15) + ((time.tm_mon) >> 4) * 10);
 ((time.tm_year) = ((time.tm_year)&15) + ((time.tm_year) >> 4) * 10);
 time.tm_mon--;
 startup_time = kernel_mktime(&time);
}

static long memory_end = 0;
static long buffer_memory_end = 0;
static long main_memory_start = 0;
static char term[32];

static char * argv_rc[] = { "/bin/sh", ((void *)0) };
static char * envp_rc[] = { "HOME=/", ((void *)0) ,((void *)0) };

static char * argv[] = { "-/bin/sh",((void *)0) };

static char * envp[] = { "HOME=/usr/root", ((void *)0), ((void *)0) };

struct drive_info { char dummy[32]; } drive_info;

int main(void)
{

 __asm__("movl %cr0,%eax \n\t"
         "xorl $6,%eax \n\t"
         "movl %eax,%cr0");
# 164 "main.c"
  ROOT_DEV = (*(unsigned short *)0x901FC);
  SWAP_DEV = (*(unsigned short *)0x901FA);
    sprintf(term, "TERM=con%dx%d", (((*(unsigned short *)0x9000e) & 0xff00) >> 8), ((*(unsigned short *)0x9000e) & 0xff));
 envp[1] = term;
 envp_rc[1] = term;
    drive_info = (*((struct drive_info *)0x90080));




 memory_end = (1 << 20) + ((*(unsigned short *)0x90002) << 10);
 memory_end &= 0xfffff000;
 if (memory_end > 16 * 1024 * 1024)
  memory_end = 16 * 1024 * 1024;

 if (memory_end > 12 * 1024 * 1024)
  buffer_memory_end = 4 * 1024 * 1024;
 else if (memory_end > 6 * 1024 * 1024)
  buffer_memory_end = 2 * 1024 * 1024;
 else
  buffer_memory_end = 1 * 1024 * 1024;

 main_memory_start = buffer_memory_end;






 mem_init(main_memory_start, memory_end);
 trap_init();
 blk_dev_init();
 chr_dev_init();
  tty_init();
 time_init();
  sched_init();
 buffer_init(buffer_memory_end);
 hd_init();
 floppy_init();
 __asm__ ("sti"::);

 Log(0, "<<<<< Linux0.12 Kernel Init Finished, Ready Start Process0 >>>>>\n");

 __asm__ ( "movl %%esp, %%eax\n\t" "pushl $0x17\n\t" "pushl %%eax\n\t" "pushfl\n\t" "pushl $0x0f\n\t" "pushl $1f\n\t" "iret\n" "1:\tmovl $0x17, %%eax\n\t" "mov %%ax, %%ds\n\t" "mov %%ax, %%es\n\t" "mov %%ax, %%fs\n\t" "mov %%ax, %%gs" :::"ax");
 if (!fork_for_process0()) {
  init();
 }
# 225 "main.c"
 for(;;)
  __asm__("int $0x80"::"a" (29):);
}




int printf(const char *fmt, ...)
{
 va_list args;
 int i;

 (args = ((char *) &(fmt) + (((sizeof (fmt) + sizeof (int) - 1) / sizeof (int)) * sizeof (int))));
 write(1, printbuf, i = vsprintf(printbuf, fmt, args));
 ;
 return i;
}



void init(void)
{
 int pid, i, fd;


 setup((void *) &drive_info);



 (void) open("/dev/tty1", 02, 0);
 (void) dup(0);
 (void) dup(0);

 printf("<<<<< Process 1 console fd = %d >>>>>\n", fd);

 printf("<<<<< %d buffers = %d bytes buffer space >>>>>\n\r", nr_buffers,
   nr_buffers * 1024);
 printf("<<<<< Free mem: %d bytes >>>>>\n\r", memory_end - main_memory_start);





 if (!(pid = fork())) {
  close(0);
  if (open("/etc/rc", 00, 0))
   _exit(1);
  execve("/bin/sh", argv_rc, envp_rc);
  _exit(2);
    }


   if (pid > 0)
  while (pid != wait(&i));





 while (1) {
  if ((pid = fork()) < 0) {
   printf("Fork failed in init %c\r\n", ' ');
   continue;
  }
  if (!pid) {
   close(0); close(1); close(2);
   setsid();
   (void) open("/dev/tty1", 02, 0);
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
 _exit(0);
}
