#include "pthread_impl.h"

int pthread_barrier_wait(pthread_barrier_t *b)
{
	int cur;

	/* Trivial case: count was set at 1 */
	if (!b->_b_limit) return PTHREAD_BARRIER_SERIAL_THREAD;

	/* Wait for anyone still suspended at previous use of barrier */
	while ((cur=b->_b_left))
		__wait(&b->_b_left, &b->_b_waiters, cur, 0);

	/* If we are the last to reach barrier, reset it and wake others */
	if (a_fetch_add(&b->_b_count, 1) == b->_b_limit) {
		b->_b_left = b->_b_limit;
		b->_b_count = 0;
		__wake(&b->_b_count, -1, 0);
		return PTHREAD_BARRIER_SERIAL_THREAD;
	}

	/* Wait for our peers to reach the barrier */
	while ((cur=b->_b_count))
		__wait(&b->_b_count, 0, cur, 0);

	/* If we're the last to wake up and barrier is awaiting reuse */
	if (a_fetch_add(&b->_b_left, -1) == 1 && b->_b_waiters)
		__wake(&b->_b_left, -1, 0);

	return 0;
}
