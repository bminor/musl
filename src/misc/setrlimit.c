#include <sys/resource.h>
#include "syscall.h"
#include "libc.h"

int setrlimit(int resource, const struct rlimit *rlim)
{
	long k_rlim[2] = { rlim->rlim_cur, rlim->rlim_max };
	return syscall(SYS_setrlimit, resource, k_rlim);
}

LFS64(setrlimit);
