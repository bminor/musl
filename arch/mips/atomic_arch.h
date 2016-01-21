#define a_cas a_cas
static inline int a_cas(volatile int *p, int t, int s)
{
	int dummy;
	__asm__ __volatile__(
		".set push\n"
		".set mips2\n"
		".set noreorder\n"
		"	sync\n"
		"1:	ll %0, %2\n"
		"	bne %0, %3, 1f\n"
		"	addu %1, %4, $0\n"
		"	sc %1, %2\n"
		"	beq %1, $0, 1b\n"
		"	nop\n"
		"	sync\n"
		"1:	\n"
		".set pop\n"
		: "=&r"(t), "=&r"(dummy), "+m"(*p) : "r"(t), "r"(s) : "memory" );
        return t;
}

#define a_swap a_swap
static inline int a_swap(volatile int *x, int v)
{
	int old, dummy;
	__asm__ __volatile__(
		".set push\n"
		".set mips2\n"
		".set noreorder\n"
		"	sync\n"
		"1:	ll %0, %2\n"
		"	addu %1, %3, $0\n"
		"	sc %1, %2\n"
		"	beq %1, $0, 1b\n"
		"	nop\n"
		"	sync\n"
		".set pop\n"
		: "=&r"(old), "=&r"(dummy), "+m"(*x) : "r"(v) : "memory" );
        return old;
}

#define a_fetch_add a_fetch_add
static inline int a_fetch_add(volatile int *x, int v)
{
	int old, dummy;
	__asm__ __volatile__(
		".set push\n"
		".set mips2\n"
		".set noreorder\n"
		"	sync\n"
		"1:	ll %0, %2\n"
		"	addu %1, %0, %3\n"
		"	sc %1, %2\n"
		"	beq %1, $0, 1b\n"
		"	nop\n"
		"	sync\n"
		".set pop\n"
		: "=&r"(old), "=&r"(dummy), "+m"(*x) : "r"(v) : "memory" );
        return old;
}
