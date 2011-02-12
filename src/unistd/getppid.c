#include <unistd.h>
#include "syscall.h"

pid_t getppid(void)
{
	return syscall0(__NR_getppid);
}
