#include <sys/wait.h>
#include <sys/resource.h>
#define SYSCALL_STANDALONE
#include "syscall.h"

pid_t wait4(pid_t, int *, int, struct rusage *);

pid_t wait3(int *status, int options, struct rusage *usage)
{
	return wait4(-1, status, options, usage);
}
