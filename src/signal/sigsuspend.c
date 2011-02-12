#include <signal.h>
#include "syscall.h"

int sigsuspend(const sigset_t *mask)
{
	return syscall2(__NR_rt_sigsuspend, (long)mask, sizeof(sigset_t));
}
