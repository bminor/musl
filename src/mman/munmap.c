#include <unistd.h>
#include <sys/mman.h>
#include "syscall.h"
#include "libc.h"

int __munmap(void *start, size_t len)
{
	return syscall(SYS_munmap, start, len);
}

weak_alias(__munmap, munmap);
