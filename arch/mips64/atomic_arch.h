#define a_ll a_ll
static inline int a_ll(volatile int *p)
{
	int v;
	__asm__ __volatile__ (
		"ll %0, %1"
		: "=r"(v) : "m"(*p));
	return v;
}

#define a_sc a_sc
static inline int a_sc(volatile int *p, int v)
{
	int r;
	__asm__ __volatile__ (
		"sc %0, %1"
		: "=r"(r), "=m"(*p) : "0"(v) : "memory");
	return r;
}

#define a_ll_p a_ll_p
static inline void *a_ll_p(volatile long *p)
{
	void *v;
	__asm__ __volatile__ (
		"lld %0, %1"
		: "=r"(v) : "m"(*p));
	return v;
}

#define a_sc_p a_sc_p
static inline int a_sc_p(volatile long *p, void *v)
{
	int r;
	__asm__ __volatile__ (
		"scd %0, %1"
		: "=r"(r), "=m"(*p) : "0"(v) : "memory");
	return r;
}

#define a_barrier a_barrier
static inline void a_barrier()
{
	/* mips2 sync, but using too many directives causes
	 * gcc not to inline it, so encode with .long instead. */
	__asm__ __volatile__ (".long 0xf" : : : "memory");
}

#define a_pre_llsc a_barrier
#define a_post_llsc a_barrier
