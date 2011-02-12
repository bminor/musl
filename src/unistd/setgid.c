#include <unistd.h>
#include "syscall.h"
#include "libc.h"

int setgid(gid_t gid)
{
	if (libc.rsyscall) return libc.rsyscall(__NR_setgid32, gid, 0, 0, 0, 0, 0);
	return syscall1(__NR_setgid32, gid);
}
