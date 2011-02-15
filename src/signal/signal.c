#include <signal.h>
#include <stddef.h>
#include "syscall.h"
#include "libc.h"

int __sigaction(int, const struct sigaction *, struct sigaction *);

void (*signal(int sig, void (*func)(int)))(int)
{
	struct sigaction sa = { .sa_handler = func, .sa_flags = SA_RESTART };
	if (__sigaction(sig, &sa, &sa) < 0)
		return SIG_ERR;
	return sa.sa_handler;
}

weak_alias(signal, bsd_signal);
