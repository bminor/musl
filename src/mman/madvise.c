#include <sys/mman.h>
#include "syscall.h"
#include "libc.h"

int __madvise(void *addr, size_t len, int advice)
{
	return syscall3(__NR_madvise, (long)addr, len, advice);
}

weak_alias(__madvise, madvise);
