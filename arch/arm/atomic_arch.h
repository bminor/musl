__attribute__((__visibility__("hidden")))
extern const void *__arm_atomics[3]; /* gettp, cas, barrier */

#if ((__ARM_ARCH_6__ || __ARM_ARCH_6K__ || __ARM_ARCH_6ZK__) && !__thumb__) \
 || __ARM_ARCH_7A__ || __ARM_ARCH_7R__ ||  __ARM_ARCH >= 7

#define a_ll a_ll
static inline int a_ll(volatile int *p)
{
	int v;
	__asm__ __volatile__ ("ldrex %0, %1" : "=r"(v) : "Q"(*p));
	return v;
}

#define a_sc a_sc
static inline int a_sc(volatile int *p, int v)
{
	int r;
	__asm__ __volatile__ ("strex %0,%2,%1" : "=&r"(r), "=Q"(*p) : "r"(v) : "memory");
	return !r;
}

#if __ARM_ARCH_7A__ || __ARM_ARCH_7R__ ||  __ARM_ARCH >= 7

#define a_barrier a_barrier
static inline void a_barrier()
{
	__asm__ __volatile__ ("dmb ish" : : : "memory");
}

#endif

#define a_pre_llsc a_barrier
#define a_post_llsc a_barrier

#else

#define a_cas a_cas
static inline int a_cas(volatile int *p, int t, int s)
{
	for (;;) {
		register int r0 __asm__("r0") = t;
		register int r1 __asm__("r1") = s;
		register volatile int *r2 __asm__("r2") = p;
		int old;
		__asm__ __volatile__ (
			"bl __a_cas"
			: "+r"(r0) : "r"(r1), "r"(r2)
			: "memory", "r3", "lr", "ip", "cc" );
		if (!r0) return t;
		if ((old=*p)!=t) return old;
	}
}

#endif

#ifndef a_barrier
#define a_barrier a_barrier
static inline void a_barrier()
{
	__asm__ __volatile__("bl __a_barrier"
		: : : "memory", "cc", "ip", "lr" );
}
#endif

#define a_crash a_crash
static inline void a_crash()
{
	__asm__ __volatile__(
#ifndef __thumb__
		".word 0xe7f000f0"
#else
		".short 0xdeff"
#endif
		: : : "memory");
}
