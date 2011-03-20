#include "syscall.h"

int brk(void *end)
{
	return -(syscall(SYS_brk, end) == -1);
}
