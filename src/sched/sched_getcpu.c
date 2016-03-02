#define _GNU_SOURCE
#include <sched.h>
#include "syscall.h"

int sched_getcpu(void)
{
	int r;
	unsigned cpu;

	r = __syscall(SYS_getcpu, &cpu, 0, 0);
	if (!r) return cpu;
	return __syscall_ret(r);
}
