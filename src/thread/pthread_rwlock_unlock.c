#include "pthread_impl.h"

int pthread_rwlock_unlock(pthread_rwlock_t *rw)
{
	struct pthread *self = pthread_self();
	if (rw->__owner == self->tid) {
		rw->__owner = 0;
		a_store(&rw->__wrlock, 0);
		if (rw->__waiters)
			__wake(&rw->__wrlock, -1, 0);
		return 0;
	}
	a_dec(&rw->__readers);
	if (rw->__waiters && !rw->__readers)
		__wake(&rw->__readers, 1, 0);
	return 0;
}
