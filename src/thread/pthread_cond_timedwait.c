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
	int r, e, tid;

	if (ts && ts->tv_nsec >= 1000000000UL)
		return EINVAL;

	pthread_testcancel();

	a_inc(&c->_c_waiters);
	c->_c_block = tid = pthread_self()->tid;

	if ((r=pthread_mutex_unlock(m))) return r;

	do e = __timedwait(&c->_c_block, tid, c->_c_clock, ts, cleanup, &cm, 0);
	while (c->_c_block == tid && (!e || e==EINTR));
	if (e == EINTR) e = 0;

	a_dec(&c->_c_waiters);

	if ((r=pthread_mutex_lock(m))) return r;

	pthread_testcancel();
	return e;
}
