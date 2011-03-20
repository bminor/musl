#include <sys/mman.h>
#include "syscall.h"

int mprotect(void *addr, size_t len, int prot)
{
	return syscall(SYS_mprotect, addr, len, prot);
}
