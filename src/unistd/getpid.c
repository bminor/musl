#include <unistd.h>
#include "syscall.h"

pid_t getpid(void)
{
	return syscall(SYS_getpid);
}
