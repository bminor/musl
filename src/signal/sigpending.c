#include <signal.h>
#include "syscall.h"

int sigpending(sigset_t *set)
{
	return syscall2(__NR_rt_sigpending, (long)set, SYSCALL_SIGSET_SIZE);
}
