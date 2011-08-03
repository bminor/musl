#include "pthread_impl.h"

static void relock(void *m)
{
	pthread_mutex_lock(m);
}

int pthread_cond_timedwait(pthread_cond_t *c, pthread_mutex_t *m, const struct timespec *ts)
{
	int r, e=0;

	if (ts && ts->tv_nsec >= 1000000000UL)
		return EINVAL;

	pthread_testcancel();

	c->_c_block = 1;
	if ((r=pthread_mutex_unlock(m))) return r;

	do e = __timedwait(&c->_c_block, 1, c->_c_clock, ts, relock, m, 0);
	while (e == EINTR);

	if ((r=pthread_mutex_lock(m))) return r;

	pthread_testcancel();
	return e;
}
