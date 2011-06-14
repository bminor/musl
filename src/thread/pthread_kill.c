#include "pthread_impl.h"

int pthread_kill(pthread_t t, int sig)
{
	int r;
	__lock(&t->killlock);
	r = t->dead ? ESRCH : -__syscall(SYS_tgkill, t->pid, t->tid, sig);
	a_store(&t->killlock, 0);
	return r;
}
