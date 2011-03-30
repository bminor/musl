#include "pthread_impl.h"

int pthread_mutex_unlock(pthread_mutex_t *m)
{
	pthread_t self;

	if (m->_m_type != PTHREAD_MUTEX_NORMAL) {
		self = __pthread_self();
		if ((m->_m_lock&0x1fffffff) != self->tid)
		 	return EPERM;
		if ((m->_m_type&3) == PTHREAD_MUTEX_RECURSIVE && --m->_m_count)
			return 0;
		if (m->_m_type >= 4) {
			self->robust_list.pending = &m->_m_next;
			*(void **)m->_m_prev = m->_m_next;
			if (m->_m_next) ((void **)m->_m_next)[-1] = m->_m_prev;
			a_store(&m->_m_lock, 0);
			self->robust_list.pending = 0;
		} else {
			a_store(&m->_m_lock, 0);
		}
	} else {
		a_store(&m->_m_lock, 0);
	}

	if (m->_m_waiters) __wake(&m->_m_lock, 1, 0);
	return 0;
}
