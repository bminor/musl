#include "pthread_impl.h"

void __lock_2(volatile int *l)
{
	if (!__syscall(SYS_futex, l, FUTEX_LOCK_PI, 0, 0))
		return;
	int old, tid = __pthread_self()->tid|INT_MIN;
	while ((old = a_cas(l, 0, tid))) {
		a_cas(l, old, old|INT_MIN);
		__syscall(SYS_futex, l, FUTEX_WAIT, old|INT_MIN, 0);
	}
}

void __lock(volatile int *l)
{
	if (a_cas(l, 0, __pthread_self()->tid)) __lock_2(l);
}

void __unlock_2(volatile int *l)
{
	if (__syscall(SYS_futex, l, FUTEX_UNLOCK_PI)) {
		*l = 0;
		__syscall(SYS_futex, l, FUTEX_WAKE, 1);
	}
}

void __unlock(volatile int *l)
{
	int old = *l;
	if (!(old & INT_MIN) && a_cas(l, old, 0)==old) return;
	__unlock_2(l);
}
