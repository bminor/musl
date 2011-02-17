#include "pthread_impl.h"

int pthread_rwlock_tryrdlock(pthread_rwlock_t *rw)
{
	a_inc(&rw->_rw_readers);
	if (rw->_rw_wrlock) {
		a_dec(&rw->_rw_readers);
		if (rw->_rw_waiters && !rw->_rw_readers)
			__wake(&rw->_rw_readers, 1, 0);
		return EAGAIN;
	}
	return 0;
}
