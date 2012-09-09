#define __SYSCALL_LL_E(x) \
((union { long long ll; long l[2]; }){ .ll = x }).l[0], \
((union { long long ll; long l[2]; }){ .ll = x }).l[1]
#define __SYSCALL_LL_O(x) 0, __SYSCALL_LL_E((x))

#define __SYSCALL_SSLEN 16

#ifndef __clang__

#define __asm_syscall(...) do { \
	register long r2 __asm__("$2"); \
	register long r7 __asm__("$7"); \
	__asm__ __volatile__ ( \
	"move $2,%2 ; syscall" \
	: "=&r"(r2), "=r"(r7) : __VA_ARGS__ \
	: "$1", "$3", "$8", "$9", "$10", "$11", "$12", "$13", \
	  "$14", "$15", "$24", "hi", "lo", "memory"); \
	return r7 ? -r2 : r2; \
	} while (0)

static inline long __syscall0(long n)
{
	register long r25 __asm__("$25") = n;
	__asm_syscall("r"(r25));
}

static inline long __syscall1(long n, long a)
{
	register long r25 __asm__("$25") = n;
	register long r4 __asm__("$4") = a;
	__asm_syscall("r"(r25), "r"(r4));
}

static inline long __syscall2(long n, long a, long b)
{
	register long r25 __asm__("$25") = n;
	register long r4 __asm__("$4") = a;
	register long r5 __asm__("$5") = b;
	__asm_syscall("r"(r25), "r"(r4), "r"(r5));
}

static inline long __syscall3(long n, long a, long b, long c)
{
	register long r25 __asm__("$25") = n;
	register long r4 __asm__("$4") = a;
	register long r5 __asm__("$5") = b;
	register long r6 __asm__("$6") = c;
	__asm_syscall("r"(r25), "r"(r4), "r"(r5), "r"(r6));
}

static inline long __syscall4(long n, long a, long b, long c, long d)
{
	register long r25 __asm__("$25") = n;
	register long r4 __asm__("$4") = a;
	register long r5 __asm__("$5") = b;
	register long r6 __asm__("$6") = c;
	register long r7 __asm__("$7") = d;
	__asm_syscall("r"(r25), "r"(r4), "r"(r5), "r"(r6), "r"(r7));
}

#else

static inline long __syscall0(long n)
{
	return (__syscall)(n);
}

static inline long __syscall1(long n, long a)
{
	return (__syscall)(n, a);
}

static inline long __syscall2(long n, long a, long b)
{
	return (__syscall)(n, a, b);
}

static inline long __syscall3(long n, long a, long b, long c)
{
	return (__syscall)(n, a, b, c);
}

static inline long __syscall4(long n, long a, long b, long c, long d)
{
	return (__syscall)(n, a, b, c, d);
}

#endif

static inline long __syscall5(long n, long a, long b, long c, long d, long e)
{
	return (__syscall)(n, a, b, c, d, e);
}

static inline long __syscall6(long n, long a, long b, long c, long d, long e, long f)
{
	return (__syscall)(n, a, b, c, d, e, f);
}
