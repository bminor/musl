#include "pthread_impl.h"

int pthread_mutex_trylock(pthread_mutex_t *m)
{
	int tid;

	if (m->_m_type == PTHREAD_MUTEX_NORMAL)
		return (m->_m_lock || a_swap(&m->_m_lock, 1)) ? EBUSY : 0;

	tid = pthread_self()->tid;

	if (m->_m_lock == tid && m->_m_type == PTHREAD_MUTEX_RECURSIVE) {
		if ((unsigned)m->_m_count >= INT_MAX) return EAGAIN;
		m->_m_count++;
		return 0;
	}

	if (m->_m_lock || a_cas(&m->_m_lock, 0, tid)) return EBUSY;
	m->_m_count = 1;
	return 0;
}
