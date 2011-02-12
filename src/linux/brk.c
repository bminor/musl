#include "syscall.h"

int brk(void *end)
{
	return -(syscall1(__NR_brk, (long)end) == -1);
}
