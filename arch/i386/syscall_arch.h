#define __SYSCALL_LL_E(x) \
((union { long long ll; long l[2]; }){ .ll = x }).l[0], \
((union { long long ll; long l[2]; }){ .ll = x }).l[1]
#define __SYSCALL_LL_O(x) __SYSCALL_LL_E((x))

#define __SYSCALL_SSLEN 8

static inline long __syscall0(long n)
{
	unsigned long __ret;
	__asm__ __volatile__ ("int $128" : "=a"(__ret) : "a"(n) : "memory");
	return __ret;
}

#ifndef __PIC__

static inline long __syscall1(long n, long a1)
{
	unsigned long __ret;
	__asm__ __volatile__ ("int $128" : "=a"(__ret) : "a"(n), "b"(a1) : "memory");
	return __ret;
}

static inline long __syscall2(long n, long a1, long a2)
{
	unsigned long __ret;
	__asm__ __volatile__ ("int $128" : "=a"(__ret) : "a"(n), "b"(a1), "c"(a2) : "memory");
	return __ret;
}

static inline long __syscall3(long n, long a1, long a2, long a3)
{
	unsigned long __ret;
	__asm__ __volatile__ ("int $128" : "=a"(__ret) : "a"(n), "b"(a1), "c"(a2), "d"(a3) : "memory");
	return __ret;
}

static inline long __syscall4(long n, long a1, long a2, long a3, long a4)
{
	unsigned long __ret;
	__asm__ __volatile__ ("int $128" : "=a"(__ret) : "a"(n), "b"(a1), "c"(a2), "d"(a3), "S"(a4) : "memory");
	return __ret;
}

static inline long __syscall5(long n, long a1, long a2, long a3, long a4, long a5)
{
	unsigned long __ret;
	__asm__ __volatile__ ("int $128" : "=a"(__ret) : "a"(n), "b"(a1), "c"(a2), "d"(a3), "S"(a4), "D"(a5) : "memory");
	return __ret;
}

static inline long __syscall6(long n, long a1, long a2, long a3, long a4, long a5, long a6)
{
	unsigned long __ret;
	__asm__ __volatile__ ("pushl %7 ; pushl %%ebp ; mov 4(%%esp),%%ebp ; int $128 ; popl %%ebp ; popl %%ecx"
		: "=a"(__ret) : "a"(n), "b"(a1), "c"(a2), "d"(a3), "S"(a4), "D"(a5), "g"(a6) : "memory");
	return __ret;
}

#else

static inline long __syscall1(long n, long a1)
{
	unsigned long __ret;
	__asm__ __volatile__ ("xchg %2,%%ebx ; int $128 ; xchg %2,%%ebx"
		: "=a"(__ret) : "a"(n), "d"(a1) : "memory");
	return __ret;
}

static inline long __syscall2(long n, long a1, long a2)
{
	unsigned long __ret;
	__asm__ __volatile__ ("xchg %2,%%ebx ; int $128 ; xchg %2,%%ebx"
		: "=a"(__ret) : "a"(n), "d"(a1), "c"(a2) : "memory");
	return __ret;
}

static inline long __syscall3(long n, long a1, long a2, long a3)
{
	unsigned long __ret;
	__asm__ __volatile__ ("xchg %2,%%ebx ; int $128 ; xchg %2,%%ebx"
		: "=a"(__ret) : "a"(n), "S"(a1), "c"(a2), "d"(a3) : "memory");
	return __ret;
}

static inline long __syscall4(long n, long a1, long a2, long a3, long a4)
{
	unsigned long __ret;
	__asm__ __volatile__ ("xchg %2,%%ebx ; int $128 ; xchg %2,%%ebx"
		: "=a"(__ret) : "a"(n), "D"(a1), "c"(a2), "d"(a3), "S"(a4) : "memory");
	return __ret;
}

#if 0
static inline long __syscall5(long n, long a1, long a2, long a3, long a4, long a5)
{
	unsigned long __ret;
	__asm__ __volatile__ ("pushl %2 ; pushl %%ebx ; mov 4(%%esp),%%ebx ; int $128 ; popl %%ebx ; popl %%ecx"
		: "=a"(__ret) : "a"(n), "g"(a1), "c"(a2), "d"(a3), "S"(a4), "D"(a5) : "memory");
	return __ret;
}
#else
static inline long __syscall5(long n, long a1, long a2, long a3, long a4, long a5)
{
	return (__syscall)(n, a1, a2, a3, a4, a5);
}
#endif

static inline long __syscall6(long n, long a1, long a2, long a3, long a4, long a5, long a6)
{
	return (__syscall)(n, a1, a2, a3, a4, a5, a6);
}

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
#define __SC_accept4     18
#define __SC_recvmmsg    19
#define __SC_sendmmsg    20
