#include <stdint.h>
#include "syscall.h"

uintptr_t __brk(uintptr_t newbrk)
{
	return syscall1(__NR_brk, newbrk);
}
