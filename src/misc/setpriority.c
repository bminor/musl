#include <sys/resource.h>
#include "syscall.h"

int setpriority(int which, id_t who, int prio)
{
	return syscall3(__NR_getpriority, which, who, prio);
}
