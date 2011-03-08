#include "pthread_impl.h"

int pthread_mutex_trylock(pthread_mutex_t *m)
{
	pthread_t self;
	if (m->_m_type != PTHREAD_MUTEX_NORMAL) {
		self = pthread_self();
		if (m->_m_type == PTHREAD_MUTEX_RECURSIVE
		 && m->_m_owner == self->tid) {
			if ((unsigned)m->_m_count >= INT_MAX) return EAGAIN;
			m->_m_count++;
			return 0;
		}
	}

	if (a_xchg(&m->_m_lock, 1))
		if (m->_m_type == PTHREAD_MUTEX_ERRORCHECK
		 && m->_m_owner == self->tid) return EDEADLK;
		else return EBUSY;
	if (m->_m_type != PTHREAD_MUTEX_NORMAL) {
		m->_m_owner = self->tid;
		m->_m_count = 1;
	}
	return 0;
}
