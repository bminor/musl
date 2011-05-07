#include "stdio_impl.h"
#include "pthread_impl.h"

void __lockfile(FILE *f)
{
	int spins=10000;
	int tid;

	if (f->lock < 0) return;
	tid = __pthread_self()->tid;
	if (f->lock == tid) {
		while (f->lockcount == INT_MAX);
		f->lockcount++;
		return;
	}
	while (a_cas(&f->lock, 0, tid))
		if (spins) spins--, a_spin();
		else __syscall(SYS_sched_yield);
	f->lockcount = 1;
}
