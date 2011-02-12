#include <unistd.h>
#include <sys/mman.h>
#include "syscall.h"

int msync(void *start, size_t len, int flags)
{
	return syscall3(__NR_msync, (long)start, len, flags);
}
