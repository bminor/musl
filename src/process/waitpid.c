#include <sys/wait.h>
#include "syscall.h"
#include "libc.h"

pid_t waitpid(pid_t pid, int *status, int options)
{
	int r;
	CANCELPT_BEGIN;
	r = syscall(SYS_wait4, pid, status, options, 0);
	if (r<0) CANCELPT_TRY;
	CANCELPT_END;
	return r;
}
