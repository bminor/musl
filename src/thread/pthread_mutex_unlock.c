#include "pthread_impl.h"

void __vm_lock_impl(int);
void __vm_unlock_impl(void);

int pthread_mutex_unlock(pthread_mutex_t *m)
{
	pthread_t self;
	int waiters = m->_m_waiters;
	int cont;
	int type = m->_m_type & 15;
	int priv = (m->_m_type & 128) ^ 128;

	if (type != PTHREAD_MUTEX_NORMAL) {
		if (!m->_m_lock)
			return EPERM;
		self = __pthread_self();
		if ((m->_m_lock&0x7fffffff) != self->tid)
			return EPERM;
		if ((type&3) == PTHREAD_MUTEX_RECURSIVE && m->_m_count)
			return m->_m_count--, 0;
		if (!priv) {
			self->robust_list.pending = &m->_m_next;
			__vm_lock_impl(+1);
		}
		*(void **)m->_m_prev = m->_m_next;
		if (m->_m_next) ((void **)m->_m_next)[-1] = m->_m_prev;
	}
	cont = a_swap(&m->_m_lock, 0);
	if (type != PTHREAD_MUTEX_NORMAL && !priv) {
		self->robust_list.pending = 0;
		__vm_unlock_impl();
	}
	if (waiters || cont<0)
		__wake(&m->_m_lock, 1, priv);
	return 0;
}
