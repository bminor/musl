#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include "syscall.h"
#include "pthread_impl.h"
#include "libc.h"

void __restore(), __restore_rt();

static pthread_t dummy(void) { return 0; }
weak_alias(dummy, __pthread_self_def);

int __libc_sigaction(int sig, const struct sigaction *sa, struct sigaction *old)
{
	struct {
		void *handler;
		unsigned long flags;
		void (*restorer)(void);
		sigset_t mask;
	} ksa, kold;
	long pksa=0, pkold=0;
	if (sa) {
		ksa.handler = sa->sa_handler;
		ksa.flags = sa->sa_flags | SA_RESTORER;
		ksa.restorer = (sa->sa_flags & SA_SIGINFO) ? __restore_rt : __restore;
		ksa.mask = sa->sa_mask;
		pksa = (long)&ksa;
	}
	if (old) pkold = (long)&kold;
	__pthread_self_def();
	if (syscall(SYS_rt_sigaction, sig, pksa, pkold, 8))
		return -1;
	if (old) {
		old->sa_handler = kold.handler;
		old->sa_flags = kold.flags;
		old->sa_mask = kold.mask;
	}
	return 0;
}

int __sigaction(int sig, const struct sigaction *sa, struct sigaction *old)
{
	if (sig-32U < 3) {
		errno = EINVAL;
		return -1;
	}
	return __libc_sigaction(sig, sa, old);
}

weak_alias(__sigaction, sigaction);
