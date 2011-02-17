#include "pthread_impl.h"

int pthread_rwlock_timedrdlock(pthread_rwlock_t *rw, const struct timespec *at)
{
	int w=0;
	while (pthread_rwlock_tryrdlock(rw)) {
		if (!w) a_inc(&rw->_rw_waiters), w++;
		if (__timedwait(&rw->_rw_wrlock, 1, CLOCK_REALTIME, at, 0)==ETIMEDOUT) {
			if (w) a_dec(&rw->_rw_waiters);
			return ETIMEDOUT;
		}
	}
	if (w) a_dec(&rw->_rw_waiters);
	return 0;
}
