#include <semaphore.h>
#include "pthread_impl.h"

static void cleanup(void *p)
{
	a_dec(p);
}

int sem_timedwait(sem_t *sem, const struct timespec *at)
{
	int r;

	if (a_fetch_add(sem->__val, -1) > 0) return 0;
	a_inc(sem->__val);

	if (at && at->tv_nsec >= 1000000000UL) {
		errno = EINVAL;
		return -1;
	}

	a_inc(sem->__val+1);
	pthread_cleanup_push(cleanup, sem->__val+1)

	CANCELPT_BEGIN;
	for (;;) {
		r = 0;
		if (!sem_trywait(sem)) break;
		r = __timedwait(sem->__val, 0, CLOCK_REALTIME, at, 0);
		if (r) {
			errno = r;
			r = -1;
			break;
		}
		CANCELPT_TRY;
	}
	CANCELPT_END;

	pthread_cleanup_pop(1);

	return r;
}
