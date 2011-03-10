#include <signal.h>
#include "syscall.h"
#include "libc.h"

int sigsuspend(const sigset_t *mask)
{
	int ret;
	CANCELPT_BEGIN;
	ret = syscall2(__NR_rt_sigsuspend, (long)mask, SYSCALL_SIGSET_SIZE);
	CANCELPT_END;
	return ret;
}
