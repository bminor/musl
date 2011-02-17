#include "pthread_impl.h"

int pthread_mutex_trylock(pthread_mutex_t *m)
{
	if (m->_m_type == PTHREAD_MUTEX_RECURSIVE) {
		pthread_t self = pthread_self();
		if (m->_m_owner == self->tid) {
			if ((unsigned)m->_m_lock >= INT_MAX) return EAGAIN;
			a_inc(&m->_m_lock);
			return 0;
		}
		if (a_fetch_add(&m->_m_lock, 1)) {
			if (a_fetch_add(&m->_m_lock, -1)==1 && m->_m_waiters)
				__wake(&m->_m_lock, 1, 0);
			return EBUSY;
		}
		m->_m_owner = self->tid;
		return 0;
	}

	if (a_xchg(&m->_m_lock, 1))
		if (m->_m_type == PTHREAD_MUTEX_ERRORCHECK
		 && m->_m_owner == pthread_self()->tid) return EDEADLK;
		else return EBUSY;
	if (m->_m_type == PTHREAD_MUTEX_ERRORCHECK)
		m->_m_owner = pthread_self()->tid;
	return 0;
}
