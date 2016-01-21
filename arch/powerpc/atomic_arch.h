#define a_cas a_cas
static inline int a_cas(volatile int *p, int t, int s)
{
	__asm__("\n"
		"	sync\n"
		"1:	lwarx %0, 0, %4\n"
		"	cmpw %0, %2\n"
		"	bne 1f\n"
		"	stwcx. %3, 0, %4\n"
		"	bne- 1b\n"
		"	isync\n"
		"1:	\n"
		: "=&r"(t), "+m"(*p) : "r"(t), "r"(s), "r"(p) : "cc", "memory" );
        return t;
}
