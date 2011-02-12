#include <unistd.h>
#include "syscall.h"

/* FIXME: add support for atfork stupidity */

pid_t fork(void)
{
	return syscall0(__NR_fork);
}
