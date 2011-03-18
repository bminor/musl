#include "pthread_impl.h"

int pthread_mutex_timedlock(pthread_mutex_t *m, const struct timespec *at)
{
	int r, w=0;
	while ((r=pthread_mutex_trylock(m)) == EBUSY) {
		if (!(r=m->_m_lock) || (r&0x40000000)) continue;
		if (!w) a_inc(&m->_m_waiters), w++;
		if (__timedwait(&m->_m_lock, r, CLOCK_REALTIME, at, 0) == ETIMEDOUT) {
			if (w) a_dec(&m->_m_waiters);
			return ETIMEDOUT;
		}
	}
	if (w) a_dec(&m->_m_waiters);
	return r;
}
