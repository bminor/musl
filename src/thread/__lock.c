#include "pthread_impl.h"

void __lock(volatile int *l)
{
	int spins=10000;
	/* Do not use futexes because we insist that unlocking is a simple
	 * assignment to optimize non-pathological code with no contention. */
	while (a_swap(l, 1))
		if (spins) spins--, a_spin();
		else __syscall(SYS_sched_yield);
}
