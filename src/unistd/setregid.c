#include <unistd.h>
#include "syscall.h"
#include "libc.h"

int setregid(gid_t rgid, gid_t egid)
{
	if (libc.rsyscall) return libc.rsyscall(__NR_setregid32, rgid, egid, 0, 0, 0, 0);
	return syscall2(__NR_setregid32, rgid, egid);
}
