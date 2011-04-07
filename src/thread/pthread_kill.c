#include "pthread_impl.h"

int pthread_kill(pthread_t t, int sig)
{
	return -__syscall(SYS_tgkill, t->pid, t->tid, sig);
}
