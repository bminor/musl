#include <signal.h>
#include <stdlib.h>

int sigpause(int sig)
{
	sigset_t mask;

	if (sigprocmask(0, NULL, &mask) < 0 || sigdelset(&mask, sig) < 0)
		return -1;
	return sigsuspend(&mask);
}
