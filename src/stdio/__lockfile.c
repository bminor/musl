#include "stdio_impl.h"
#include "pthread_impl.h"

void __lockfile(FILE *f)
{
	int spins;
	if (f->owner < 0) return;
	if (f->owner && f->owner == __pthread_self()->tid) {
		while (f->lockcount == INT_MAX);
		f->lockcount++;
		return;
	}
	spins = 100000;
	while (a_swap(&f->lock, 1))
		if (spins) spins--, a_spin();
		else syscall0(__NR_sched_yield);
	f->owner = __pthread_self()->tid;
	f->lockcount = 1;
}
