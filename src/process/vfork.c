#include <unistd.h>
#include "syscall.h"

pid_t vfork(void)
{
	/* vfork syscall cannot be made from C code */
	return syscall(SYS_fork);
}
