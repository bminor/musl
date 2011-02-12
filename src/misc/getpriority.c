#include <sys/resource.h>
#include "syscall.h"

int getpriority(int which, id_t who)
{
	int ret = syscall2(__NR_getpriority, which, who);
	if (ret < 0) return ret;
	return 20-ret;
}
