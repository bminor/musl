#include <stdint.h>
#include "syscall.h"

uintptr_t __brk(uintptr_t newbrk)
{
	return syscall(SYS_brk, newbrk);
}
