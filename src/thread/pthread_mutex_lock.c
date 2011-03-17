#include "pthread_impl.h"

int pthread_mutex_lock(pthread_mutex_t *m)
{
	int r;
	while ((r=pthread_mutex_trylock(m)) == EBUSY) {
		if (!(r=m->_m_lock)) continue;
		if (m->_m_type == PTHREAD_MUTEX_ERRORCHECK
		 && r == pthread_self()->tid)
			return EDEADLK;
		__wait(&m->_m_lock, &m->_m_waiters, r, 0);
	}
	return r;
}
