#include <unistd.h>
#include <sys/resource.h>
#include "syscall.h"

int nice(int inc)
{
#ifdef __NR_nice
	return syscall1(__NR_nice, inc);
#else
	return setpriority(PRIO_PROCESS, 0, getpriority(PRIO_PROCESS, 0)+inc);
#endif
}
