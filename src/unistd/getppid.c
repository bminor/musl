#include <unistd.h>
#include "syscall.h"

pid_t getppid(void)
{
	return syscall(SYS_getppid);
}
