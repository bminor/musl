#include "pthread_impl.h"

struct cm {
	pthread_cond_t *c;
	pthread_mutex_t *m;
};

static void unwait(pthread_cond_t *c, pthread_mutex_t *m)
{
	int w;

	/* Removing a waiter is non-trivial if we could be using requeue
	 * based broadcast signals, due to mutex access issues, etc. */

	if (c->_c_mutex == (void *)-1) {
		a_dec(&c->_c_waiters);
		return;
	}

	while (a_swap(&c->_c_lock, 1))
		__wait(&c->_c_lock, &c->_c_lockwait, 1, 1);

	/* Atomically decrement waiters2 if positive, else mutex waiters. */
	do w = c->_c_waiters2;
	while (w && a_cas(&c->_c_waiters2, w, w-1)!=w);
	if (!w) a_dec(&m->_m_waiters);

	a_store(&c->_c_lock, 0);
	if (c->_c_lockwait) __wake(&c->_c_lock, 1, 1);
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
	int r, e=0, seq;

	if (ts && ts->tv_nsec >= 1000000000UL)
		return EINVAL;

	pthread_testcancel();

	if (c->_c_mutex != (void *)-1) c->_c_mutex = m;

	a_inc(&c->_c_waiters);
	a_inc(&c->_c_waiters2);

	seq = c->_c_seq;

	if ((r=pthread_mutex_unlock(m))) return r;

	do e = __timedwait(&c->_c_seq, seq, c->_c_clock, ts, cleanup, &cm, 0);
	while (c->_c_seq == seq && (!e || e==EINTR));
	if (e == EINTR) e = 0;

	unwait(c, m);

	if ((r=pthread_mutex_lock(m))) return r;

	return e;
}
