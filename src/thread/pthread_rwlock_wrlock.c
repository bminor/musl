#include "pthread_impl.h"

int pthread_rwlock_wrlock(pthread_rwlock_t *rw)
{
	int nr;
	while (pthread_rwlock_trywrlock(rw)==EAGAIN) {
		if ((nr=rw->__readers))
			__wait(&rw->__readers, &rw->__waiters, nr, 0);
		else
			__wait(&rw->__wrlock, &rw->__waiters, 1, 0);
	}
	return 0;
}
