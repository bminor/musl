#include <unistd.h>
#include "syscall.h"
#include "libc.h"

int setreuid(uid_t ruid, uid_t euid)
{
	if (libc.rsyscall) return libc.rsyscall(__NR_setreuid32, ruid, euid, 0, 0, 0, 0);
	return syscall2(__NR_setreuid32, ruid, euid);
}
