#include "pthread_impl.h"

static void relock(void *m)
{
	pthread_mutex_lock(m);
}

int pthread_cond_timedwait(pthread_cond_t *c, pthread_mutex_t *m, const struct timespec *ts)
{
	int r, e=0;
	CANCELPT_BEGIN;
	CANCELPT_END;

	pthread_cleanup_push(relock, m);
	c->_c_block = 1;
	if ((r=pthread_mutex_unlock(m))) return r;

	CANCELPT_BEGIN;
	do e = __timedwait(&c->_c_block, 1, c->_c_clock, ts, 0);
	while (e == EINTR);
	CANCELPT_END;

	pthread_cleanup_pop(0);
	if ((r=pthread_mutex_lock(m))) return r;

	CANCELPT_BEGIN;
	CANCELPT_END;
	return e;
}
