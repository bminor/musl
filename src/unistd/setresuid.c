#define _GNU_SOURCE
#include <unistd.h>
#include "syscall.h"
#include "libc.h"

int setresuid(uid_t ruid, uid_t euid, uid_t suid)
{
	return __rsyscall(SYS_setresuid, ruid, euid, suid, 0, 0, 0);
}
