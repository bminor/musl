#include <signal.h>
#include <errno.h>

int sigismember(const sigset_t *set, int sig)
{
	unsigned s = sig-1;
	if (s >= 8*sizeof(sigset_t) || sig-32U<3) {
		errno = EINVAL;
		return -1;
	}
	return !!(set->__bits[s/8/sizeof *set->__bits] & 1UL<<(s&8*sizeof *set->__bits-1));
}
