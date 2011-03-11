#include <semaphore.h>
#include "pthread_impl.h"

int sem_timedwait(sem_t *sem, const struct timespec *at)
{
	int val;

	for (;;) {
		if (a_fetch_add(sem->__val, -1) > 0) return 0;
		val = a_fetch_add(sem->__val, 1)+1;
		if (val==1) __wake(sem->__val, 1, 0);
		CANCELPT_BEGIN;
		if (at && at->tv_nsec >= 1000000000UL) {
			errno = EINVAL;
			return -1;
		}
		if (val <= 0 && __timedwait(sem->__val, val, CLOCK_REALTIME, at, 0) == ETIMEDOUT) {
			errno = ETIMEDOUT;
			return -1;
		}
		CANCELPT_END;
	}
}
