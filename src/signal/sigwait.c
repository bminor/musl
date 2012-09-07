#include <signal.h>
#include <stddef.h>

int sigwait(const sigset_t *restrict mask, int *restrict sig)
{
	siginfo_t si;
	if (sigtimedwait(mask, &si, NULL) < 0)
		return -1;
	*sig = si.si_signo;
	return 0;
}
