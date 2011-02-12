#include <sys/mman.h>
#include "syscall.h"

int mprotect(void *addr, size_t len, int prot)
{
	return syscall3(__NR_mprotect, (long)addr, len, prot);
}
