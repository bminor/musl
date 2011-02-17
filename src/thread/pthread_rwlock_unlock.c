#include "pthread_impl.h"

int pthread_rwlock_unlock(pthread_rwlock_t *rw)
{
	struct pthread *self = pthread_self();
	if (rw->_rw_owner == self->tid) {
		rw->_rw_owner = 0;
		a_store(&rw->_rw_wrlock, 0);
		if (rw->_rw_waiters)
			__wake(&rw->_rw_wrlock, -1, 0);
		return 0;
	}
	a_dec(&rw->_rw_readers);
	if (rw->_rw_waiters && !rw->_rw_readers)
		__wake(&rw->_rw_readers, 1, 0);
	return 0;
}
