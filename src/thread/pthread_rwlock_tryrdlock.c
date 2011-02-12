#include "pthread_impl.h"

int pthread_rwlock_tryrdlock(pthread_rwlock_t *rw)
{
	a_inc(&rw->__readers);
	if (rw->__wrlock) {
		a_dec(&rw->__readers);
		if (rw->__waiters && !rw->__readers)
			__wake(&rw->__readers, 1, 0);
		return EAGAIN;
	}
	return 0;
}
