#include "pthread_impl.h"

int pthread_rwlock_timedwrlock(pthread_rwlock_t *rw, const struct timespec *at)
{
	int nr, *p, w=0;
	while (pthread_rwlock_trywrlock(rw)==EAGAIN) {
		if (!w) a_inc(&rw->_rw_waiters), w++;
		if ((nr=rw->_rw_readers)) p = &rw->_rw_readers;
		else nr=1, p = &rw->_rw_wrlock;
		if (__timedwait(p, nr, CLOCK_REALTIME, at, 0)==ETIMEDOUT) {
			if (w) a_dec(&rw->_rw_waiters);
			return ETIMEDOUT;
		}
	}
	if (w) a_dec(&rw->_rw_waiters);
	return 0;
}
