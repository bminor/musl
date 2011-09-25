#include "pthread_impl.h"

struct cm {
	pthread_cond_t *c;
	pthread_mutex_t *m;
};

static void unwait(pthread_cond_t *c, pthread_mutex_t *m)
{
	int w;

	/* Cannot leave waiting status if there are any live broadcasters
	 * which might be inspecting/using the mutex. */
	while ((w=c->_c_bcast)) __wait(&c->_c_bcast, &c->_c_leavers, w, 0);

	/* If the waiter count is zero, it must be the case that the
	 * caller's count has been moved to the mutex due to bcast. */
	do w = c->_c_waiters;
	while (w && a_cas(&c->_c_waiters, w, w-1)!=w);
	if (!w) a_dec(&m->_m_waiters);
}

static void cleanup(void *p)
{
	struct cm *cm = p;
	unwait(cm->c, cm->m);
	pthread_mutex_lock(cm->m);
}

int pthread_cond_timedwait(pthread_cond_t *c, pthread_mutex_t *m, const struct timespec *ts)
{
	struct cm cm = { .c=c, .m=m };
	int r, e, tid;

	if (ts && ts->tv_nsec >= 1000000000UL)
		return EINVAL;

	pthread_testcancel();

	if (c->_c_mutex != (void *)-1) c->_c_mutex = m;

	a_inc(&c->_c_waiters);
	c->_c_block = tid = pthread_self()->tid;

	if ((r=pthread_mutex_unlock(m))) return r;

	do e = __timedwait(&c->_c_block, tid, c->_c_clock, ts, cleanup, &cm, 0);
	while (c->_c_block == tid && (!e || e==EINTR));
	if (e == EINTR) e = 0;

	unwait(c, m);

	if ((r=pthread_mutex_lock(m))) return r;

	pthread_testcancel();
	return e;
}
