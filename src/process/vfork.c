#include <unistd.h>
#include "syscall.h"

pid_t vfork(void)
{
	/* vfork syscall cannot be made from C code */
	return syscall0(__NR_fork);
}
