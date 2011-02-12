#include <stddef.h>
#include "syscall.h"

void *sbrk(ptrdiff_t inc)
{
	return (void *)syscall1(__NR_brk, syscall1(__NR_brk, 0)+inc);
}
