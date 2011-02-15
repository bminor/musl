#include <unistd.h>
#include "syscall.h"
#include "libc.h"

int truncate(const char *path, off_t length)
{
	return syscall3(__NR_truncate, (long)path, SYSCALL_LL(length));
}

LFS64(truncate);
