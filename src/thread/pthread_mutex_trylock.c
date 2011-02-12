#include "pthread_impl.h"

int pthread_mutex_trylock(pthread_mutex_t *m)
{
	if (m->__type == PTHREAD_MUTEX_RECURSIVE) {
		pthread_t self = pthread_self();
		if (m->__owner == self) {
			if ((unsigned)m->__lock >= INT_MAX) return EAGAIN;
			a_inc(&m->__lock);
			return 0;
		}
		if (a_fetch_add(&m->__lock, 1)) {
			if (a_fetch_add(&m->__lock, -1)==1 && m->__waiters)
				__wake(&m->__lock, 1, 0);
			return EBUSY;
		}
		m->__owner = self;
		return 0;
	}

	if (a_xchg(&m->__lock, 1))
		if (m->__type == PTHREAD_MUTEX_ERRORCHECK
		 && m->__owner == pthread_self()) return EDEADLK;
		else return EBUSY;
	if (m->__type == PTHREAD_MUTEX_ERRORCHECK)
		m->__owner = pthread_self();
	return 0;
}
