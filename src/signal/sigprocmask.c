#include <signal.h>
#include <errno.h>
#include "syscall.h"
#include "libc.h"
#include "pthread_impl.h"

int sigprocmask(int how, const sigset_t *set, sigset_t *old)
{
	if (how > 2U) {
		errno = EINVAL;
		return -1;
	}
	return syscall(SYS_rt_sigprocmask, how, set, old, 8);
}
