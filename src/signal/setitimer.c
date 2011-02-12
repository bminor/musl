#include <sys/time.h>
#include "syscall.h"

int setitimer(int which, const struct itimerval *new, struct itimerval *old)
{
	int ret;
	long knew[4] = {
		new->it_interval.tv_sec, new->it_interval.tv_usec,
		new->it_value.tv_sec, new->it_value.tv_usec
	}, kold[4];

	if (!(ret = syscall3(__NR_setitimer, which, (long)&knew, old ? (long)&kold : 0)) && old)
		*old = (struct itimerval){ { kold[0], kold[1] }, { kold[2], kold[3] } };
	return ret;
}
