#include "stdio_impl.h"
#include "pthread_impl.h"

#define MAYBE_WAITERS 0x40000000

void flockfile(FILE *f)
{
	while (ftrylockfile(f)) {
		int owner = f->lock;
		if (!owner) continue;
		a_cas(&f->lock, owner, owner|MAYBE_WAITERS);
		__futexwait(&f->lock, owner|MAYBE_WAITERS, 1);
	}
}
