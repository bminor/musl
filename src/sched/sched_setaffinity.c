#define _GNU_SOURCE
#include <sched.h>
#include "syscall.h"

int sched_setaffinity(pid_t tid, size_t size, const cpu_set_t *set)
{
	return syscall(SYS_sched_setaffinity, tid, size, set);
}
