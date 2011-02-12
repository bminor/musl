#include "pthread_impl.h"

int pthread_mutex_timedlock(pthread_mutex_t *m, const struct timespec *at)
{
	int r, w=0;
	while ((r=pthread_mutex_trylock(m)) == EBUSY) {
		if (!w) a_inc(&m->__waiters), w++;
		if (__timedwait(&m->__lock, 1, CLOCK_REALTIME, at, 0) == ETIMEDOUT) {
			if (w) a_dec(&m->__waiters);
			return ETIMEDOUT;
		}
	}
	if (w) a_dec(&m->__waiters);
	return r;
}
