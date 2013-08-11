#define _GNU_SOURCE
#include <sched.h>
#include "syscall.h"

int sched_getaffinity(pid_t tid, size_t size, cpu_set_t *set)
{
	long ret = __syscall(SYS_sched_getaffinity, tid, size, set);
	if (ret > 0) ret = 0;
	return __syscall_ret(ret);
}
