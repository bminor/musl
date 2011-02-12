#include "futex.h"
#include "syscall.h"

int __futex(volatile int *addr, int op, int val, void *ts)
{
	return syscall4(__NR_futex, (long)addr, op, val, (long)ts);
}

