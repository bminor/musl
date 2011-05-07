#include "pthread_impl.h"

struct instance
{
	int count;
	int last;
	int waiters;
	int finished;
};

int pthread_barrier_wait(pthread_barrier_t *b)
{
	int limit = b->_b_limit;
	struct instance *inst;

	/* Trivial case: count was set at 1 */
	if (!limit) return PTHREAD_BARRIER_SERIAL_THREAD;

	/* Otherwise we need a lock on the barrier object */
	while (a_swap(&b->_b_lock, 1))
		__wait(&b->_b_lock, &b->_b_waiters, 1, 0);
	inst = b->_b_inst;

	/* First thread to enter the barrier becomes the "instance owner" */
	if (!inst) {
		struct instance new_inst = { 0 };
		int spins = 10000;
		b->_b_inst = inst = &new_inst;
		a_store(&b->_b_lock, 0);
		if (b->_b_waiters) __wake(&b->_b_lock, 1, 0);
		while (spins-- && !inst->finished)
			a_spin();
		a_inc(&inst->finished);
		while (inst->finished == 1)
			__syscall(SYS_futex, &inst->finished, FUTEX_WAIT,1,0);
		return PTHREAD_BARRIER_SERIAL_THREAD;
	}

	/* Last thread to enter the barrier wakes all non-instance-owners */
	if (++inst->count == limit) {
a_spin(); a_spin();
		b->_b_inst = 0;
		a_store(&b->_b_lock, 0);
		if (b->_b_waiters) __wake(&b->_b_lock, 1, 0);
		a_store(&inst->last, 1);
		if (inst->waiters)
			__wake(&inst->last, -1, 0);
	} else {
		a_store(&b->_b_lock, 0);
		if (b->_b_waiters) __wake(&b->_b_lock, 1, 0);
		__wait(&inst->last, &inst->waiters, 0, 0);
	}

	/* Last thread to exit the barrier wakes the instance owner */
	if (a_fetch_add(&inst->count,-1)==1 && a_fetch_add(&inst->finished,1))
		__wake(&inst->finished, 1, 0);

	return 0;
}
