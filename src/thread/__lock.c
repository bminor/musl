#define SYSCALL_RETURN_ERRNO
#include "pthread_impl.h"

void __lock(volatile int *l)
{
	int spins=100000;
	/* Do not use futexes because we insist that unlocking is a simple
	 * assignment to optimize non-pathological code with no contention. */
	while (a_xchg(l, 1))
		if (spins) spins--, a_spin();
		else syscall0(__NR_sched_yield);
}
