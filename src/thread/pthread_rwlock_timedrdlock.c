#include "pthread_impl.h"

int pthread_rwlock_timedrdlock(pthread_rwlock_t *rw, const struct timespec *at)
{
	int w=0;
	while (pthread_rwlock_tryrdlock(rw)) {
		if (!w) a_inc(&rw->__waiters), w++;
		if (__timedwait(&rw->__wrlock, 1, CLOCK_REALTIME, at, 0)==ETIMEDOUT) {
			if (w) a_dec(&rw->__waiters);
			return ETIMEDOUT;
		}
	}
	if (w) a_dec(&rw->__waiters);
	return 0;
}
