#define a_ctz_64 a_ctz_64
static inline int a_ctz_64(uint64_t x)
{
	int r;
	__asm__( "bsf %1,%0 ; jnz 1f ; bsf %2,%0 ; addl $32,%0\n1:"
		: "=&r"(r) : "r"((unsigned)x), "r"((unsigned)(x>>32)) );
	return r;
}

#define a_ctz_l a_ctz_l
static inline int a_ctz_l(unsigned long x)
{
	long r;
	__asm__( "bsf %1,%0" : "=r"(r) : "r"(x) );
	return r;
}

#define a_and_64 a_and_64
static inline void a_and_64(volatile uint64_t *p, uint64_t v)
{
	__asm__( "lock ; andl %1, (%0) ; lock ; andl %2, 4(%0)"
		: : "r"((long *)p), "r"((unsigned)v), "r"((unsigned)(v>>32)) : "memory" );
}

#define a_or_64 a_or_64
static inline void a_or_64(volatile uint64_t *p, uint64_t v)
{
	__asm__( "lock ; orl %1, (%0) ; lock ; orl %2, 4(%0)"
		: : "r"((long *)p), "r"((unsigned)v), "r"((unsigned)(v>>32)) : "memory" );
}

#define a_or_l a_or_l
static inline void a_or_l(volatile void *p, long v)
{
	__asm__( "lock ; orl %1, %0"
		: "=m"(*(long *)p) : "r"(v) : "memory" );
}

#define a_cas a_cas
static inline int a_cas(volatile int *p, int t, int s)
{
	__asm__( "lock ; cmpxchg %3, %1"
		: "=a"(t), "=m"(*p) : "a"(t), "r"(s) : "memory" );
	return t;
}

#define a_or a_or
static inline void a_or(volatile int *p, int v)
{
	__asm__( "lock ; orl %1, %0"
		: "=m"(*p) : "r"(v) : "memory" );
}

#define a_and a_and
static inline void a_and(volatile int *p, int v)
{
	__asm__( "lock ; andl %1, %0"
		: "=m"(*p) : "r"(v) : "memory" );
}

#define a_swap a_swap
static inline int a_swap(volatile int *x, int v)
{
	__asm__( "xchg %0, %1" : "=r"(v), "=m"(*x) : "0"(v) : "memory" );
	return v;
}

#define a_fetch_add a_fetch_add
static inline int a_fetch_add(volatile int *x, int v)
{
	__asm__( "lock ; xadd %0, %1" : "=r"(v), "=m"(*x) : "0"(v) : "memory" );
	return v;
}

#define a_inc a_inc
static inline void a_inc(volatile int *x)
{
	__asm__( "lock ; incl %0" : "=m"(*x) : "m"(*x) : "memory" );
}

#define a_dec a_dec
static inline void a_dec(volatile int *x)
{
	__asm__( "lock ; decl %0" : "=m"(*x) : "m"(*x) : "memory" );
}

#define a_store a_store
static inline void a_store(volatile int *p, int x)
{
	__asm__( "movl %1, %0 ; lock ; orl $0,(%%esp)" : "=m"(*p) : "r"(x) : "memory" );
}

#define a_spin a_spin
static inline void a_spin()
{
	__asm__ __volatile__( "pause" : : : "memory" );
}

#define a_barrier a_barrier
static inline void a_barrier()
{
	__asm__ __volatile__( "" : : : "memory" );
}

#define a_crash a_crash
static inline void a_crash()
{
	__asm__ __volatile__( "hlt" : : : "memory" );
}
