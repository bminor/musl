#define _GNU_SOURCE
#include <unistd.h>
#include "syscall.h"
#include "libc.h"

int setresgid(gid_t rgid, gid_t egid, gid_t sgid)
{
	if (libc.rsyscall) return libc.rsyscall(__NR_setresgid, rgid, egid, sgid, 0, 0, 0);
	return syscall(SYS_setresgid, rgid, egid, sgid);
}
