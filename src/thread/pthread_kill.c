#include "pthread_impl.h"

int pthread_kill(pthread_t t, int sig)
{
	return __syscall(__NR_tgkill, t->pid, t->tid, sig);
}
