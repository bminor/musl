#include <signal.h>
#include "syscall.h"
#include "libc.h"

int sigsuspend(const sigset_t *mask)
{
	int ret;
	CANCELPT_BEGIN;
	ret = syscall(__NR_rt_sigsuspend, mask, 8);
	CANCELPT_END;
	return ret;
}
