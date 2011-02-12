#include <signal.h>
#include "syscall.h"
#include "libc.h"

int __libc_sigprocmask(int how, const sigset_t *set, sigset_t *old)
{
	return syscall4(__NR_rt_sigprocmask, how, (long)set, (long)old, 8);
}

int __sigprocmask(int how, const sigset_t *set, sigset_t *old)
{
	sigset_t tmp;
	/* Quickly mask out bits 32 and 33 (thread control signals) */
	if (0 && how != SIG_UNBLOCK && (set->__bits[4/sizeof *set->__bits] & 3UL<<(32&8*sizeof *set->__bits-1))) {
		tmp = *set;
		set = &tmp;
		tmp.__bits[4/sizeof *set->__bits] &= ~(3UL<<(32&8*sizeof *set->__bits-1));
	}
	return __libc_sigprocmask(how, set, old);
}

weak_alias(__sigprocmask, sigprocmask);
weak_alias(__sigprocmask, pthread_sigmask);
