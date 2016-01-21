#if __ARM_ARCH_7A__ || __ARM_ARCH_7R__ ||  __ARM_ARCH >= 7

#define a_barrier a_barrier
static inline void a_barrier()
{
	__asm__ __volatile__("dmb ish");
}

#define a_cas a_cas
static inline int a_cas(volatile int *p, int t, int s)
{
	int old;
	__asm__ __volatile__(
		"	dmb ish\n"
		"1:	ldrex %0,%3\n"
		"	cmp %0,%1\n"
		"	bne 1f\n"
		"	strex %0,%2,%3\n"
		"	cmp %0, #0\n"
		"	bne 1b\n"
		"	mov %0, %1\n"
		"1:	dmb ish\n"
		: "=&r"(old)
		: "r"(t), "r"(s), "Q"(*p)
		: "memory", "cc" );
	return old;
}

#define a_swap a_swap
static inline int a_swap(volatile int *x, int v)
{
	int old, tmp;
	__asm__ __volatile__(
		"	dmb ish\n"
		"1:	ldrex %0,%3\n"
		"	strex %1,%2,%3\n"
		"	cmp %1, #0\n"
		"	bne 1b\n"
		"	dmb ish\n"
		: "=&r"(old), "=&r"(tmp)
		: "r"(v), "Q"(*x)
		: "memory", "cc" );
	return old;
}

#define a_fetch_add a_fetch_add
static inline int a_fetch_add(volatile int *x, int v)
{
	int old, tmp;
	__asm__ __volatile__(
		"	dmb ish\n"
		"1:	ldrex %0,%3\n"
		"	add %0,%0,%2\n"
		"	strex %1,%0,%3\n"
		"	cmp %1, #0\n"
		"	bne 1b\n"
		"	dmb ish\n"
		: "=&r"(old), "=&r"(tmp)
		: "r"(v), "Q"(*x)
		: "memory", "cc" );
	return old-v;
}

#define a_inc a_inc
static inline void a_inc(volatile int *x)
{
	int tmp, tmp2;
	__asm__ __volatile__(
		"	dmb ish\n"
		"1:	ldrex %0,%2\n"
		"	add %0,%0,#1\n"
		"	strex %1,%0,%2\n"
		"	cmp %1, #0\n"
		"	bne 1b\n"
		"	dmb ish\n"
		: "=&r"(tmp), "=&r"(tmp2)
		: "Q"(*x)
		: "memory", "cc" );
}

#define a_dec a_dec
static inline void a_dec(volatile int *x)
{
	int tmp, tmp2;
	__asm__ __volatile__(
		"	dmb ish\n"
		"1:	ldrex %0,%2\n"
		"	sub %0,%0,#1\n"
		"	strex %1,%0,%2\n"
		"	cmp %1, #0\n"
		"	bne 1b\n"
		"	dmb ish\n"
		: "=&r"(tmp), "=&r"(tmp2)
		: "Q"(*x)
		: "memory", "cc" );
}

#define a_and a_and
static inline void a_and(volatile int *x, int v)
{
	int tmp, tmp2;
	__asm__ __volatile__(
		"	dmb ish\n"
		"1:	ldrex %0,%3\n"
		"	and %0,%0,%2\n"
		"	strex %1,%0,%3\n"
		"	cmp %1, #0\n"
		"	bne 1b\n"
		"	dmb ish\n"
		: "=&r"(tmp), "=&r"(tmp2)
		: "r"(v), "Q"(*x)
		: "memory", "cc" );
}

#define a_or a_or
static inline void a_or(volatile int *x, int v)
{
	int tmp, tmp2;
	__asm__ __volatile__(
		"	dmb ish\n"
		"1:	ldrex %0,%3\n"
		"	orr %0,%0,%2\n"
		"	strex %1,%0,%3\n"
		"	cmp %1, #0\n"
		"	bne 1b\n"
		"	dmb ish\n"
		: "=&r"(tmp), "=&r"(tmp2)
		: "r"(v), "Q"(*x)
		: "memory", "cc" );
}

#define a_store a_store
static inline void a_store(volatile int *p, int x)
{
	__asm__ __volatile__(
		"	dmb ish\n"
		"	str %1,%0\n"
		"	dmb ish\n"
		: "=m"(*p)
		: "r"(x)
		: "memory", "cc" );
}

#else

int __a_cas(int, int, volatile int *) __attribute__((__visibility__("hidden")));
#define __k_cas __a_cas

#define a_barrier a_barrier
static inline void a_barrier()
{
	__asm__ __volatile__("bl __a_barrier"
		: : : "memory", "cc", "ip", "lr" );
}

#define a_cas a_cas
static inline int a_cas(volatile int *p, int t, int s)
{
	int old;
	for (;;) {
		if (!__k_cas(t, s, p))
			return t;
		if ((old=*p) != t)
			return old;
	}
}

#endif
