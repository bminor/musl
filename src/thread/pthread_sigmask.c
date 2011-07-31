#include <signal.h>
#include <errno.h>
#include <pthread.h>
#include "syscall.h"

int pthread_sigmask(int how, const sigset_t *set, sigset_t *old)
{
	if (how > 2U) return EINVAL;
	return -__syscall(SYS_rt_sigprocmask, how, set, old, 8);
}
