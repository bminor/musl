#define _GNU_SOURCE
#include <unistd.h>
#include "syscall.h"
#include "libc.h"

int setresuid(uid_t ruid, uid_t euid, uid_t suid)
{
	if (libc.rsyscall) return libc.rsyscall(__NR_setresuid, ruid, euid, suid, 0, 0, 0);
	return syscall(SYS_setresuid, ruid, euid, suid);
}
