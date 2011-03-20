#include "pthread_impl.h"

int pthread_mutex_trylock(pthread_mutex_t *m)
{
	int tid;
	int own;
	pthread_t self;

	if (m->_m_type == PTHREAD_MUTEX_NORMAL)
		return (m->_m_lock || a_swap(&m->_m_lock, 1)) ? EBUSY : 0;

	self = pthread_self();
	tid = self->tid | 0x80000000;

	if (m->_m_type >= 4) {
		if (!self->robust_list.off)
			syscall(SYS_set_robust_list,
				&self->robust_list, 3*sizeof(long));
		self->robust_list.off = (char*)&m->_m_lock-(char *)&m->_m_next;
		self->robust_list.pending = &m->_m_next;
	}

	if (m->_m_lock == tid && (m->_m_type&3) == PTHREAD_MUTEX_RECURSIVE) {
		if ((unsigned)m->_m_count >= INT_MAX) return EAGAIN;
		m->_m_count++;
		return 0;
	}

	own = m->_m_lock;
	if ((own && !(own & 0x40000000)) || a_cas(&m->_m_lock, own, tid)!=own)
		return EBUSY;

	m->_m_count = 1;

	if (m->_m_type < 4) return 0;

	if (m->_m_type >= 8) {
		m->_m_lock = 0;
		return ENOTRECOVERABLE;
	}
	m->_m_next = self->robust_list.head;
	m->_m_prev = &self->robust_list.head;
	if (self->robust_list.head)
		self->robust_list.head[-1] = &m->_m_next;
	self->robust_list.head = &m->_m_next;
	self->robust_list.pending = 0;
	if (own) {
		m->_m_type += 8;
		return EOWNERDEAD;
	}

	return 0;
}
