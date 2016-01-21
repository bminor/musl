#define a_ll a_ll
static inline int a_ll(volatile int *p)
{
	int v;
	__asm__ __volatile__ ("ldxr %0, %1" : "=r"(v) : "Q"(*p));
	return v;
}

#define a_sc a_sc
static inline int a_sc(volatile int *p, int v)
{
	int r;
	__asm__ __volatile__ ("stxr %w0,%1,%2" : "=&r"(r) : "r"(v), "Q"(*p) : "memory");
	return !r;
}

#define a_barrier a_barrier
static inline void a_barrier()
{
	__asm__ __volatile__ ("dmb ish" : : : "memory");
}

#define a_pre_llsc a_barrier
#define a_post_llsc a_barrier

#define a_cas_p a_cas_p
static inline void *a_cas_p(volatile void *p, void *t, void *s)
{
	void *old;
	__asm__ __volatile__(
		"	dmb ish\n"
		"1:	ldxr %0,%3\n"
		"	cmp %0,%1\n"
		"	b.ne 1f\n"
		"	stxr %w0,%2,%3\n"
		"	cbnz %w0,1b\n"
		"	mov %0,%1\n"
		"1:	dmb ish\n"
		: "=&r"(old)
		: "r"(t), "r"(s), "Q"(*(void *volatile *)p)
		: "memory", "cc");
	return old;
}

#define a_ctz_64 a_ctz_64
static inline int a_ctz_64(uint64_t x)
{
	__asm__(
		"	rbit %0, %1\n"
		"	clz %0, %0\n"
		: "=r"(x) : "r"(x));
	return x;
}
