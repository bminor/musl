#include "pthread_impl.h"
#include <stddef.h>

void __do_private_robust_list()
{
	pthread_t self = __pthread_self();
	void **p, **prev, **next;
	pthread_mutex_t *m;

	for (prev=0, p=self->robust_list.head; p; p=next) {
		next = *p;
		m = (void *)((char *)p - offsetof(pthread_mutex_t, _m_next));
		if (!(m->_m_type & 128)) {
			int waiters = m->_m_waiters;
			if (prev) *prev = next;
			else self->robust_list.head = next;
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
