#include "pthread_impl.h"

static void relock(void *m)
{
	pthread_mutex_lock(m);
}

int pthread_cond_timedwait(pthread_cond_t *c, pthread_mutex_t *m, const struct timespec *ts)
{
	int r, e=0;

	pthread_testcancel();

	pthread_cleanup_push(relock, m);
	c->_c_block = 1;
	if ((r=pthread_mutex_unlock(m))) return r;

	do e = __timedwait_cp(&c->_c_block, 1, c->_c_clock, ts, 0);
	while (e == EINTR);

	pthread_cleanup_pop(0);
	if ((r=pthread_mutex_lock(m))) return r;

	pthread_testcancel();
	return e;
}
