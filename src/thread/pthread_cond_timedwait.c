#include "pthread_impl.h"

static void relock(void *m)
{
	pthread_mutex_lock(m);
}

int pthread_cond_timedwait(pthread_cond_t *c, pthread_mutex_t *m, const struct timespec *ts)
{
	int r, e=0;
	CANCELPT(0);

	pthread_cleanup_push(relock, m);
	c->__block = 1;
	if ((r=pthread_mutex_unlock(m))) return r;

	CANCELPT(1);
	e = __timedwait(&c->__block, 1, CLOCK_REALTIME, ts, 0);
	CANCELPT(0);

	pthread_cleanup_pop(0);
	if ((r=pthread_mutex_lock(m))) return r;

	CANCELPT(0);
	return e;
}
