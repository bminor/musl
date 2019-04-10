#define __SYSCALL_LL_E(x) \
((union { long long ll; long l[2]; }){ .ll = x }).l[0], \
((union { long long ll; long l[2]; }){ .ll = x }).l[1]
#define __SYSCALL_LL_O(x) __SYSCALL_LL_E((x))

#if SYSCALL_NO_TLS
#define SYSCALL_INSNS "int $128"
#else
#define SYSCALL_INSNS "call *%%gs:16"
#endif

#define SYSCALL_INSNS_12 "xchg %%ebx,%%edx ; " SYSCALL_INSNS " ; xchg %%ebx,%%edx"
#define SYSCALL_INSNS_34 "xchg %%ebx,%%edi ; " SYSCALL_INSNS " ; xchg %%ebx,%%edi"

static inline long __syscall0(long n)
{
	unsigned long __ret;
	__asm__ __volatile__ (SYSCALL_INSNS : "=a"(__ret) : "a"(n) : "memory");
	return __ret;
}

static inline long __syscall1(long n, long a1)
{
	unsigned long __ret;
	__asm__ __volatile__ (SYSCALL_INSNS_12 : "=a"(__ret) : "a"(n), "d"(a1) : "memory");
	return __ret;
}

static inline long __syscall2(long n, long a1, long a2)
{
	unsigned long __ret;
	__asm__ __volatile__ (SYSCALL_INSNS_12 : "=a"(__ret) : "a"(n), "d"(a1), "c"(a2) : "memory");
	return __ret;
}

static inline long __syscall3(long n, long a1, long a2, long a3)
{
	unsigned long __ret;
	__asm__ __volatile__ (SYSCALL_INSNS_34 : "=a"(__ret) : "a"(n), "D"(a1), "c"(a2), "d"(a3) : "memory");
	return __ret;
}

static inline long __syscall4(long n, long a1, long a2, long a3, long a4)
{
	unsigned long __ret;
	__asm__ __volatile__ (SYSCALL_INSNS_34 : "=a"(__ret) : "a"(n), "D"(a1), "c"(a2), "d"(a3), "S"(a4) : "memory");
	return __ret;
}

static inline long __syscall5(long n, long a1, long a2, long a3, long a4, long a5)
{
	unsigned long __ret, __tmp;
	__asm__ __volatile__ ("mov %%ebx,%1 ; mov %3,%%ebx ; " SYSCALL_INSNS " ; mov %1,%%ebx"
		: "=a"(__ret), "=m"(__tmp) : "a"(n), "g"(a1), "c"(a2), "d"(a3), "S"(a4), "D"(a5) : "memory");
	return __ret;
}

static inline long __syscall6(long n, long a1, long a2, long a3, long a4, long a5, long a6)
{
	unsigned long __ret, __tmp1, __tmp2;
	__asm__ __volatile__ ("mov %%ebx,%1 ; mov %%ebp,%2 ; mov %4,%%ebx ; mov %9,%%ebp ; " SYSCALL_INSNS " ; mov %2,%%ebp ; mov %1,%%ebx"
		: "=a"(__ret), "=m"(__tmp1), "=m"(__tmp2) : "a"(n), "g"(a1), "c"(a2), "d"(a3), "S"(a4), "D"(a5), "g"(a6) : "memory");
	return __ret;
}

#define VDSO_USEFUL
#define VDSO_CGT_SYM "__vdso_clock_gettime"
#define VDSO_CGT_VER "LINUX_2.6"

#define SYSCALL_USE_SOCKETCALL
