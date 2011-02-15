#include <sys/resource.h>
#include "syscall.h"
#include "libc.h"

int getrlimit(int resource, struct rlimit *rlim)
{
	long k_rlim[2];
	if (syscall2(__NR_getrlimit, resource, (long)k_rlim) < 0)
		return -1;
	rlim->rlim_cur = k_rlim[0] == -1 ? -1 : (unsigned long)k_rlim[0];
	rlim->rlim_max = k_rlim[1] == -1 ? -1 : (unsigned long)k_rlim[1];
	return 0;
}

LFS64(getrlimit);
