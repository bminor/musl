#include "pthread_impl.h"

static void unlock(pthread_cond_t *c)
{
	a_dec(&c->_c_bcast);
	if (c->_c_leavers) __wake(&c->_c_bcast, -1, 0);
}

int pthread_cond_broadcast(pthread_cond_t *c)
{
	pthread_mutex_t *m;
	int w;

	if (!c->_c_waiters) return 0;
	a_inc(&c->_c_bcast);
	if (!c->_c_waiters) {
		unlock(c);
		return 0;
	}

	a_store(&c->_c_block, 0);

	m = c->_c_mutex;

	/* If mutex ptr is not available, simply wake all waiters. */
	if (m == (void *)-1) {
		unlock(c);
		__wake(&c->_c_block, -1, 0);
		return 0;
	}

	/* Move waiter count to the mutex */
	for (;;) {
		w = c->_c_waiters;
		a_fetch_add(&m->_m_waiters, w);
		if (a_cas(&c->_c_waiters, w, 0) == w) break;
		a_fetch_add(&m->_m_waiters, -w);
	}

	/* Perform the futex requeue, waking one waiter unless we know
	 * that the calling thread holds the mutex. */
	__syscall(SYS_futex, &c->_c_block, FUTEX_REQUEUE,
		!m->_m_type || (m->_m_lock&INT_MAX)!=pthread_self()->tid,
		INT_MAX, &m->_m_lock);

	unlock(c);
	return 0;
}
