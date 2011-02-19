#define _GNU_SOURCE
#include <sys/wait.h>
#include <sys/resource.h>
#include <string.h>
#include "syscall.h"

pid_t wait4(pid_t pid, int *status, int options, struct rusage *usage)
{
	return syscall4(__NR_wait4, pid, (long)status, options, (long)usage);
}
