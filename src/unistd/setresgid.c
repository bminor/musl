#define _GNU_SOURCE
#include <unistd.h>
#include "syscall.h"
#include "libc.h"

int setresgid(gid_t rgid, gid_t egid, gid_t sgid)
{
	return __rsyscall(SYS_setresgid, rgid, egid, sgid, 0, 0, 0);
}
