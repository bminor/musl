#include <unistd.h>
#include "syscall.h"

pid_t getpgid(pid_t pid)
{
	return syscall1(__NR_getpgid, pid);
}
