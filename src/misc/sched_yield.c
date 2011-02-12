#include <sched.h>
#include "syscall.h"
#include "libc.h"

int __yield()
{
	return syscall0(__NR_sched_yield);
}

weak_alias(__yield, sched_yield);
