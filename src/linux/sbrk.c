#include <stddef.h>
#include "syscall.h"

void *sbrk(ptrdiff_t inc)
{
	unsigned long cur = syscall(SYS_brk, 0);
	if (inc && syscall(SYS_brk, cur+inc) != cur+inc) return (void *)-1;
	return (void *)cur;
}
