#include <signal.h>
#include <errno.h>
#include "syscall.h"
#include "libc.h"
#include "pthread_impl.h"

int __libc_sigprocmask(int how, const sigset_t *set, sigset_t *old)
{
	return syscall(SYS_rt_sigprocmask, how, set, old, 8);
}

int __sigprocmask(int how, const sigset_t *set, sigset_t *old)
{
	sigset_t tmp;
	if (how > 2U) {
		errno = EINVAL;
		return -1;
	}
	/* Disallow blocking thread control signals */
	if (set && how != SIG_UNBLOCK) {
		tmp = *set;
		set = &tmp;
		sigdelset(&tmp, SIGCANCEL);
		sigdelset(&tmp, SIGSYSCALL);
		sigdelset(&tmp, SIGTIMER);
	}
	return __libc_sigprocmask(how, set, old);
}

weak_alias(__sigprocmask, sigprocmask);
