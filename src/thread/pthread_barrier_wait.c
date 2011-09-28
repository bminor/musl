#include "pthread_impl.h"

static int vmlock[2];

void __vm_lock(int inc)
{
	for (;;) {
		int v = vmlock[0];
		if (inc*v < 0) __wait(vmlock, vmlock+1, v, 1);
		else if (a_cas(vmlock, v, v+inc)==v) break;
	}
}

void __vm_unlock(void)
{
	if (vmlock[0]>0) a_dec(vmlock);
	else a_inc(vmlock);
	if (vmlock[1]) __wake(vmlock, 1, 1);
}

static int pshared_barrier_wait(pthread_barrier_t *b)
{
	int limit = (b->_b_limit & INT_MAX) + 1;
	int seq;
	int ret = 0;

	if (limit==1) return PTHREAD_BARRIER_SERIAL_THREAD;

	while (a_swap(&b->_b_lock, 1))
		__wait(&b->_b_lock, &b->_b_waiters, 1, 0);

	seq = b->_b_seq;

	if (++b->_b_count == limit) {
		ret = PTHREAD_BARRIER_SERIAL_THREAD;
		b->_b_seq++;
		__wake(&b->_b_seq, -1, 0);
	} else {
		a_store(&b->_b_lock, 0);
		if (b->_b_waiters) __wake(&b->_b_lock, 1, 0);
		__wait(&b->_b_seq, 0, seq, 0);
	}

	__vm_lock(+1);

	if (a_fetch_add(&b->_b_count, -1)==1) {
		b->_b_seq++;
		__wake(&b->_b_seq, -1, 0);
		a_store(&b->_b_lock, 0);
		if (b->_b_waiters) __wake(&b->_b_lock, 1, 0);
	} else {
		__wait(&b->_b_seq, 0, seq+1, 0);
	}
	
	__vm_unlock();

	return ret;
}

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

	/* Process-shared barriers require a separate, inefficient wait */
	if (limit < 0) return pshared_barrier_wait(b);

	/* Otherwise we need a lock on the barrier object */
	while (a_swap(&b->_b_lock, 1))
		__wait(&b->_b_lock, &b->_b_waiters, 1, 1);
	inst = b->_b_inst;

	/* First thread to enter the barrier becomes the "instance owner" */
	if (!inst) {
		struct instance new_inst = { 0 };
		int spins = 10000;
		b->_b_inst = inst = &new_inst;
		a_store(&b->_b_lock, 0);
		if (b->_b_waiters) __wake(&b->_b_lock, 1, 1);
		while (spins-- && !inst->finished)
			a_spin();
		a_inc(&inst->finished);
		while (inst->finished == 1)
			__syscall(SYS_futex, &inst->finished, FUTEX_WAIT,1,0);
		return PTHREAD_BARRIER_SERIAL_THREAD;
	}

	/* Last thread to enter the barrier wakes all non-instance-owners */
	if (++inst->count == limit) {
		b->_b_inst = 0;
		a_store(&b->_b_lock, 0);
		if (b->_b_waiters) __wake(&b->_b_lock, 1, 1);
		a_store(&inst->last, 1);
		if (inst->waiters)
			__wake(&inst->last, -1, 1);
	} else {
		a_store(&b->_b_lock, 0);
		if (b->_b_waiters) __wake(&b->_b_lock, 1, 1);
		__wait(&inst->last, &inst->waiters, 0, 1);
	}

	/* Last thread to exit the barrier wakes the instance owner */
	if (a_fetch_add(&inst->count,-1)==1 && a_fetch_add(&inst->finished,1))
		__wake(&inst->finished, 1, 1);

	return 0;
}
