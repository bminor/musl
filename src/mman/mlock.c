#include <sys/mman.h>
#include "syscall.h"

int mlock(const void *addr, size_t len)
{
	return syscall2(__NR_mlock, (long)addr, len);
}
