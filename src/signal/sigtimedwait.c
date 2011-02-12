#include <signal.h>
#include "syscall.h"

int sigtimedwait(const sigset_t *mask, siginfo_t *si, const struct timespec *timeout)
{
	long k_timeout[2];
	if (timeout) {
		k_timeout[0] = timeout->tv_sec;
		k_timeout[1] = timeout->tv_nsec;
	}
	return syscall4(__NR_rt_sigtimedwait, (long)mask, (long)si, timeout ? (long)k_timeout : 0, SYSCALL_SIGSET_SIZE);
}
