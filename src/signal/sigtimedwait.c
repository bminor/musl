#include <signal.h>
#include <errno.h>
#include "syscall.h"

int sigtimedwait(const sigset_t *mask, siginfo_t *si, const struct timespec *timeout)
{
	int ret;
	do {
		ret = syscall4(__NR_rt_sigtimedwait, (long)mask, (long)si, (long)timeout, SYSCALL_SIGSET_SIZE);
	} while (ret<0 && errno==EINTR);
	return ret;
}
