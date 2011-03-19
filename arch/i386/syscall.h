#ifndef _SYSCALL_H
#define _SYSCALL_H

#include <sys/syscall.h>

#define SYSCALL_LL(x) \
((union { long long ll; long l[2]; }){ .ll = x }).l[0], \
((union { long long ll; long l[2]; }){ .ll = x }).l[1]

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

static inline long syscall0(long n)
{
	unsigned long ret;
	__asm__ __volatile__ ("int $128" : "=a"(ret) : "a"(n) : "memory");
	return __syscall_ret(ret);
}

#ifndef __PIC__

static inline long syscall1(long n, long a1)
{
	unsigned long ret;
	__asm__ __volatile__ ("int $128" : "=a"(ret) : "a"(n), "b"(a1) : "memory");
	return __syscall_ret(ret);
}

static inline long syscall2(long n, long a1, long a2)
{
	unsigned long ret;
	__asm__ __volatile__ ("int $128" : "=a"(ret) : "a"(n), "b"(a1), "c"(a2) : "memory");
	return __syscall_ret(ret);
}

static inline long syscall3(long n, long a1, long a2, long a3)
{
	unsigned long ret;
	__asm__ __volatile__ ("int $128" : "=a"(ret) : "a"(n), "b"(a1), "c"(a2), "d"(a3) : "memory");
	return __syscall_ret(ret);
}

static inline long syscall4(long n, long a1, long a2, long a3, long a4)
{
	unsigned long ret;
	__asm__ __volatile__ ("int $128" : "=a"(ret) : "a"(n), "b"(a1), "c"(a2), "d"(a3), "S"(a4) : "memory");
	return __syscall_ret(ret);
}

static inline long syscall5(long n, long a1, long a2, long a3, long a4, long a5)
{
	unsigned long ret;
	__asm__ __volatile__ ("int $128" : "=a"(ret) : "a"(n), "b"(a1), "c"(a2), "d"(a3), "S"(a4), "D"(a5) : "memory");
	return __syscall_ret(ret);
}

static inline long syscall6(long n, long a1, long a2, long a3, long a4, long a5, long a6)
{
	unsigned long ret;
	__asm__ __volatile__ ("pushl %%ebp ; mov %%eax,%%ebp ; movl %1,%%eax ; int $128 ; popl %%ebp"
		: "=a"(ret) : "i"(n), "b"(a1), "c"(a2), "d"(a3), "S"(a4), "D"(a5), "a"(a6) : "memory");
	return __syscall_ret(ret);
}

#else

static inline long syscall1(long n, long a1)
{
	unsigned long ret;
	__asm__ __volatile__ ("xchg %2,%%ebx ; int $128 ; xchg %2,%%ebx"
		: "=a"(ret) : "a"(n), "r"(a1) : "memory");
	return __syscall_ret(ret);
}

static inline long syscall2(long n, long a1, long a2)
{
	unsigned long ret;
	__asm__ __volatile__ ("xchg %2,%%ebx ; int $128 ; xchg %2,%%ebx"
		: "=a"(ret) : "a"(n), "r"(a1), "c"(a2) : "memory");
	return __syscall_ret(ret);
}

static inline long syscall3(long n, long a1, long a2, long a3)
{
	unsigned long ret;
	__asm__ __volatile__ ("xchg %2,%%ebx ; int $128 ; xchg %2,%%ebx"
		: "=a"(ret) : "a"(n), "r"(a1), "c"(a2), "d"(a3) : "memory");
	return __syscall_ret(ret);
}

static inline long syscall4(long n, long a1, long a2, long a3, long a4)
{
	unsigned long ret;
	__asm__ __volatile__ ("xchg %2,%%ebx ; int $128 ; xchg %2,%%ebx"
		: "=a"(ret) : "a"(n), "r"(a1), "c"(a2), "d"(a3), "S"(a4) : "memory");
	return __syscall_ret(ret);
}

static inline long syscall5(long n, long a1, long a2, long a3, long a4, long a5)
{
	unsigned long ret;
	__asm__ __volatile__ ("pushl %%ebx ; mov %%eax,%%ebx ; movl %1,%%eax ; int $128 ; popl %%ebx"
		: "=a"(ret) : "i"(n), "a"(a1), "c"(a2), "d"(a3), "S"(a4), "D"(a5) : "memory");
	return __syscall_ret(ret);
}

#define syscall6(n,a1,a2,a3,a4,a5,a6) __syscall((n),(a1),(a2),(a3),(a4),(a5),(a6))

#endif

#define __SC_socket      1
#define __SC_bind        2
#define __SC_connect     3
#define __SC_listen      4
#define __SC_accept      5
#define __SC_getsockname 6
#define __SC_getpeername 7
#define __SC_socketpair  8
#define __SC_send        9
#define __SC_recv        10
#define __SC_sendto      11
#define __SC_recvfrom    12
#define __SC_shutdown    13
#define __SC_setsockopt  14
#define __SC_getsockopt  15
#define __SC_sendmsg     16
#define __SC_recvmsg     17


#define socketcall(nm, a, b, c, d, e, f) syscall2(__NR_socketcall, __SC_##nm, \
    (long)(long [6]){ (long)a, (long)b, (long)c, (long)d, (long)e, (long)f })


#undef O_LARGEFILE
#define O_LARGEFILE 0100000

/* the following are needed for iso c functions to use */
#define __syscall_open(filename, flags, mode) syscall3(__NR_open, (long)(filename), (flags)|O_LARGEFILE, (mode))
#define __syscall_read(fd, buf, len)          syscall3(__NR_read, (fd), (long)(buf), (len))
#define __syscall_write(fd, buf, len)         syscall3(__NR_write, (fd), (long)(buf), (len))
#define __syscall_close(fd)                   syscall1(__NR_close, (fd))
#define __syscall_fcntl(fd, cmd, arg)         syscall3(__NR_fcntl64, (fd), (cmd), (long)(arg))
#define __syscall_dup2(old, new)              syscall2(__NR_dup2, (old), (new))
#define __syscall_unlink(path)                syscall1(__NR_unlink, (long)(path))
#define __syscall_getpid()                    syscall0(__NR_getpid)
#define __syscall_kill(pid,sig)               syscall2(__NR_kill, (pid), (sig))
#define __syscall_sigaction(sig,new,old)      syscall4(__NR_rt_sigaction, (sig), (long)(new), (long)(old), SYSCALL_SIGSET_SIZE)
#define __syscall_ioctl(fd,ioc,arg)           syscall3(__NR_ioctl, (fd), (ioc), (long)(arg))
#define __syscall_exit(code)                  syscall1(__NR_exit, code)

long __syscall(long, ...);

#endif
