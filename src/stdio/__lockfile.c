#include "stdio_impl.h"
#include "pthread_impl.h"

int __lockfile(FILE *f)
{
	int owner, tid = __pthread_self()->tid;
	if (f->lock == tid)
		return 0;
	while ((owner = a_cas(&f->lock, 0, tid)))
		__wait(&f->lock, &f->waiters, owner, 1);
	return 1;
}

void __unlockfile(FILE *f)
{
	a_store(&f->lock, 0);
	if (f->waiters) __wake(&f->lock, 1, 1);
}
