#include "pthread_impl.h"

int pthread_mutex_lock(pthread_mutex_t *m)
{
	int r;

	if (m->_m_type == PTHREAD_MUTEX_NORMAL && !a_swap(&m->_m_lock, EBUSY))
		return 0;

	while ((r=pthread_mutex_trylock(m)) == EBUSY) {
		if (!(r=m->_m_lock) || (r&0x40000000)) continue;
		if ((m->_m_type&3) == PTHREAD_MUTEX_ERRORCHECK
		 && (r&0x1fffffff) == pthread_self()->tid)
			return EDEADLK;
		__wait(&m->_m_lock, &m->_m_waiters, r, 0);
	}
	return r;
}
