#include "pthread_impl.h"
#include "lock.h"

int pthread_kill(pthread_t t, int sig)
{
	int r;
	sigset_t set;
	__block_app_sigs(&set);
	LOCK(t->killlock);
	r = t->tid ? -__syscall(SYS_tkill, t->tid, sig)
		: (sig+0U >= _NSIG ? EINVAL : 0);
	UNLOCK(t->killlock);
	__restore_sigs(&set);
	return r;
}
