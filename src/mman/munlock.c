#include <sys/mman.h>
#include "syscall.h"

int munlock(const void *addr, size_t len)
{
	return syscall2(__NR_munlock, (long)addr, len);
}
