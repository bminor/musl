#include <unistd.h>
#include "syscall.h"
#include "libc.h"

int setuid(uid_t uid)
{
	return __rsyscall(SYS_setuid, uid, 0, 0, 0, 0, 0);
}
