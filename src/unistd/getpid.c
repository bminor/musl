#include <unistd.h>
#include "syscall.h"

pid_t getpid(void)
{
	return syscall0(__NR_getpid);
}
