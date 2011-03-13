#include "stdio_impl.h"
#include "pthread_impl.h"

int ftrylockfile(FILE *f)
{
	libc.lockfile = __lockfile;
	if (f->owner && f->owner == pthread_self()->tid) {
		if (f->lockcount == INT_MAX)
			return -1;
		f->lockcount++;
		return 0;
	}
	if (a_swap(&f->lock, 1))
		return -1;
	f->owner = pthread_self()->tid;
	f->lockcount = 1;
	return 0;
}
