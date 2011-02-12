#include <sys/time.h>
#include "syscall.h"

int getitimer(int which, struct itimerval *old)
{
	int ret;
	long kold[4];

	if (!(ret = syscall2(__NR_getitimer, which, (long)&kold)))
		*old = (struct itimerval){ { kold[0], kold[1] }, { kold[2], kold[3] } };
	return ret;
}
