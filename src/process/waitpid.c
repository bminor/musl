#include <sys/wait.h>
#include "syscall.h"

pid_t waitpid(pid_t pid, int *status, int options)
{
	return syscall4(__NR_wait4, pid, (long)status, options, 0);
}
