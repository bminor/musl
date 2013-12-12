#include <signal.h>
#include <stddef.h>

int sigrelse(int sig)
{
	sigset_t mask;

	sigemptyset(&mask);
	if (sigaddset(&mask, sig) < 0) return -1;
	return sigprocmask(SIG_UNBLOCK, &mask, NULL);
}
