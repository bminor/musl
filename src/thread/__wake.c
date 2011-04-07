#include "pthread_impl.h"

void __wake(volatile int *addr, int cnt, int priv)
{
	if (priv) priv = 128; priv=0;
	if (cnt<0) cnt = INT_MAX;
	__syscall(SYS_futex, (long)addr, FUTEX_WAKE | priv, cnt);
}
