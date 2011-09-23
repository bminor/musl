#include "pthread_impl.h"

struct cm {
	pthread_cond_t *c;
	pthread_mutex_t *m;
};

static void cleanup(void *p)
{
	struct cm *cm = p;
	a_dec(&cm->c->_c_waiters);
	pthread_mutex_lock(cm->m);
}

int pthread_cond_timedwait(pthread_cond_t *c, pthread_mutex_t *m, const struct timespec *ts)
{
	struct cm cm = { .c=c, .m=m };
	int r, e=0;

	if (ts && ts->tv_nsec >= 1000000000UL)
		return EINVAL;

	pthread_testcancel();

	c->_c_block = 1;
	if ((r=pthread_mutex_unlock(m))) return r;

	a_inc(&c->_c_waiters);
	do e = __timedwait(&c->_c_block, 1, c->_c_clock, ts, cleanup, &cm, 0);
	while (e == EINTR);
	a_dec(&c->_c_waiters);

	if ((r=pthread_mutex_lock(m))) return r;

	pthread_testcancel();
	return e;
}
