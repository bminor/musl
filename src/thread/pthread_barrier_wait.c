#include "pthread_impl.h"

int pthread_barrier_wait(pthread_barrier_t *b)
{
	int cur;

	/* Trivial case: count was set at 1 */
	if (!b->__limit) return PTHREAD_BARRIER_SERIAL_THREAD;

	/* Wait for anyone still suspended at previous use of barrier */
	while ((cur=b->__left))
		__wait(&b->__left, &b->__waiters, cur, 0);

	/* If we are the last to reach barrier, reset it and wake others */
	if (a_fetch_add(&b->__count, 1) == b->__limit) {
		b->__left = b->__limit;
		b->__count = 0;
		__wake(&b->__count, -1, 0);
		return PTHREAD_BARRIER_SERIAL_THREAD;
	}

	/* Wait for our peers to reach the barrier */
	while ((cur=b->__count))
		__wait(&b->__count, 0, cur, 0);

	/* If we're the last to wake up and barrier is awaiting reuse */
	if (a_fetch_add(&b->__left, -1) == 1 && b->__waiters)
		__wake(&b->__left, -1, 0);

	return 0;
}
