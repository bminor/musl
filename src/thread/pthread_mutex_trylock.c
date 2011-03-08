#include "pthread_impl.h"

int pthread_mutex_trylock(pthread_mutex_t *m)
{
	int tid;

	if (m->_m_type == PTHREAD_MUTEX_NORMAL)
		return -a_xchg(&m->_m_lock, 1) & EBUSY;

	tid = pthread_self()->tid;

	if (m->_m_owner == tid) {
		if (m->_m_type != PTHREAD_MUTEX_RECURSIVE)
			return EDEADLK;
		if ((unsigned)m->_m_count >= INT_MAX) return EAGAIN;
		m->_m_count++;
		return 0;
	}

	if (a_xchg(&m->_m_lock, 1)) return EBUSY;
	m->_m_owner = tid;
	m->_m_count = 1;
	return 0;
}
