#include <unistd.h>
#include <sys/mman.h>
#include "syscall.h"
#include "libc.h"

int __munmap(void *start, size_t len)
{
	return syscall2(__NR_munmap, (long)start, len);
}

weak_alias(__munmap, munmap);
