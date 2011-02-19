#include <signal.h>
#include "syscall.h"
#include "libc.h"
#include "pthread_impl.h"

int __libc_sigprocmask(int how, const sigset_t *set, sigset_t *old)
{
	return syscall4(__NR_rt_sigprocmask, how, (long)set, (long)old, 8);
}

int __sigprocmask(int how, const sigset_t *set, sigset_t *old)
{
	sigset_t tmp;
	/* Disallow blocking thread control signals */
	if (how != SIG_UNBLOCK) {
		tmp = *set;
		set = &tmp;
		sigdelset(&tmp, SIGCANCEL);
		sigdelset(&tmp, SIGSYSCALL);
	}
	return __libc_sigprocmask(how, set, old);
}

weak_alias(__sigprocmask, sigprocmask);
weak_alias(__sigprocmask, pthread_sigmask);
