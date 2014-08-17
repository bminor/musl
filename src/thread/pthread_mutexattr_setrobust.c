#include "pthread_impl.h"
#include <stddef.h>

void __do_private_robust_list()
{
	pthread_t self = __pthread_self();
	volatile void *volatile *p;
	volatile void *volatile *prev;
	volatile void *volatile *next;
	pthread_mutex_t *m;

	prev = &self->robust_list.head;
	for (p=self->robust_list.head; p&&p!=&self->robust_list.head; p=next) {
		next = *p;
		m = (void *)((char *)p - offsetof(pthread_mutex_t, _m_next));
		if (!(m->_m_type & 128)) {
			int waiters = m->_m_waiters;
			*prev = next;
			int cont = a_swap(&m->_m_lock, self->tid|0x40000000);
			if (cont < 0 || waiters) __wake(&m->_m_lock, 1, 1);
		} else {
			prev = p;
		}
	}
}

int pthread_mutexattr_setrobust(pthread_mutexattr_t *a, int robust)
{
	if (robust > 1U) return EINVAL;
	a->__attr &= ~4;
	a->__attr |= robust*4;
	return 0;
}
