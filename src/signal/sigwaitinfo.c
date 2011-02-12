#include <signal.h>
#include <stddef.h>

int sigwaitinfo(const sigset_t *mask, siginfo_t *si)
{
	return sigtimedwait(mask, si, NULL);
}
