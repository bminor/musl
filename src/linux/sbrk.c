#include <stddef.h>
#include "syscall.h"

void *sbrk(ptrdiff_t inc)
{
	return (void *)syscall(SYS_brk, syscall(SYS_brk, 0)+inc);
}
