#include <sys/mman.h>
#include "syscall.h"

int munlockall(void)
{
	return syscall0(__NR_munlockall);
}
