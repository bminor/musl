#include "pthread_impl.h"

int pthread_mutex_unlock(pthread_mutex_t *m)
{
	if (m->_m_type != PTHREAD_MUTEX_NORMAL) {
		if (m->_m_owner != pthread_self()->tid)
		 	return EPERM;
		if (m->_m_type == PTHREAD_MUTEX_RECURSIVE && --m->_m_count)
			return 0;
	}

	m->_m_owner = 0;
	m->_m_lock = 0;
	if (m->_m_waiters) __wake(&m->_m_lock, 1, 0);
	return 0;
}
