#include <unistd.h>
#include "syscall.h"
#include "libc.h"

int setuid(uid_t uid)
{
	if (libc.rsyscall) return libc.rsyscall(__NR_setuid32, uid, 0, 0, 0, 0, 0);
	return syscall1(__NR_setuid32, uid);
}
