#ifndef _SYSCALL_H
#define _SYSCALL_H

#include <sys/syscall.h>

#define SYSCALL_LL(x) x, 0
#define SYSCALL_SIGSET_SIZE 8

#if defined(SYSCALL_NORETURN)
static inline long __syscall_ret(unsigned long r)
{
	for(;;);
	return 0;
}
#elif defined(SYSCALL_RETURN_ERRNO)
static inline long __syscall_ret(unsigned long r)
{
	return -r;
}
#else
extern long __syscall_ret(unsigned long);
#endif

// 64: di,  si,  dx,  r10, r8,  r9
// 32: ebx, ecx, edx, esi, edi, ebp

#define SYSCALL "syscall"

static inline long syscall0(long n)
{
	unsigned long ret;
	__asm__ __volatile__ (SYSCALL : "=a"(ret) : "a"(n) : "rcx", "r11", "memory");
	return __syscall_ret(ret);
}

static inline long syscall1(long n, long a1)
{
	unsigned long ret;
	__asm__ __volatile__ (SYSCALL : "=a"(ret) : "a"(n), "D"(a1) : "rcx", "r11", "memory");
	return __syscall_ret(ret);
}

static inline long syscall2(long n, long a1, long a2)
{
	unsigned long ret;
	__asm__ __volatile__ (SYSCALL : "=a"(ret) : "a"(n), "D"(a1), "S"(a2)
						  : "rcx", "r11", "memory");
	return __syscall_ret(ret);
}

static inline long syscall3(long n, long a1, long a2, long a3)
{
	unsigned long ret;
	__asm__ __volatile__ (SYSCALL : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
						  "d"(a3) : "rcx", "r11", "memory");
	return __syscall_ret(ret);
}

static inline long syscall4(long n, long a1, long a2, long a3, long a4)
{
	unsigned long ret;
	register long r10 __asm__("r10") = a4;
	__asm__ __volatile__ (SYSCALL : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
						  "d"(a3), "r"(r10): "rcx", "r11", "memory");
	return __syscall_ret(ret);
}

static inline long syscall5(long n, long a1, long a2, long a3, long a4,
							long a5)
{
	unsigned long ret;
	register long r10 __asm__("r10") = a4;
	register long r8 __asm__("r8") = a5;
	__asm__ __volatile__ (SYSCALL : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
						  "d"(a3), "r"(r10), "r"(r8) : "rcx", "r11", "memory");
	return __syscall_ret(ret);
}

static inline long syscall6(long n, long a1, long a2, long a3, long a4,
							long a5, long a6)
{
	unsigned long ret;
	register long r10 __asm__("r10") = a4;
	register long r8 __asm__("r8") = a5;
	register long r9 __asm__("r9") = a6;
	__asm__ __volatile__ (SYSCALL : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
						  "d"(a3), "r"(r10), "r"(r8), "r"(r9) : "rcx", "r11", "memory");
	return __syscall_ret(ret);
}

#undef O_LARGEFILE
#define O_LARGEFILE 0100000

#define socketcall(nm, a, b, c, d, e, f) syscall6(__NR_##nm, \
    (long)a, (long)b, (long)c, (long)d, (long)e, (long)f)

/* the following are needed for iso c functions to use */
#define __syscall_open(filename, flags, mode) syscall3(__NR_open, (long)(filename), (flags)|O_LARGEFILE, (mode))
#define __syscall_read(fd, buf, len)          syscall3(__NR_read, (fd), (long)(buf), (len))
#define __syscall_write(fd, buf, len)         syscall3(__NR_write, (fd), (long)(buf), (len))
#define __syscall_close(fd)                   syscall1(__NR_close, (fd))
#define __syscall_fcntl(fd, cmd, arg)         syscall3(__NR_fcntl, (fd), (cmd), (long)(arg))
#define __syscall_dup2(old, new)              syscall2(__NR_dup2, (old), (new))
#define __syscall_unlink(path)                syscall1(__NR_unlink, (long)(path))
#define __syscall_getpid()                    syscall0(__NR_getpid)
#define __syscall_kill(pid,sig)               syscall2(__NR_kill, (pid), (sig))
#define __syscall_sigaction(sig,new,old)      syscall4(__NR_rt_sigaction, (sig), (long)(new), (long)(old), SYSCALL_SIGSET_SIZE)
#define __syscall_ioctl(fd,ioc,arg)           syscall3(__NR_ioctl, (fd), (ioc), (long)(arg))
#define __syscall_exit(code)                  syscall1(__NR_exit, code)

long __syscall(long, ...);

#endif
