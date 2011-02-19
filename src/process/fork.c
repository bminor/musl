#include <unistd.h>
#include "syscall.h"
#include "libc.h"

pid_t fork(void)
{
	pid_t ret;
	if (libc.fork_handler) libc.fork_handler(-1);
	ret = syscall0(__NR_fork);
	if (libc.fork_handler) libc.fork_handler(!ret);
	return ret;
}
