#include <sys/resource.h>
#include "syscall.h"
#include "libc.h"

int setrlimit(int resource, const struct rlimit *rlim)
{
	long k_rlim[2] = { rlim->rlim_cur, rlim->rlim_max };
	return syscall2(__NR_setrlimit, resource, (long)k_rlim);
}

LFS64(setrlimit);
