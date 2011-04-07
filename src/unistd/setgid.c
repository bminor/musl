#include <unistd.h>
#include "syscall.h"
#include "libc.h"

int setgid(gid_t gid)
{
	return __rsyscall(__NR_setgid, gid, 0, 0, 0, 0, 0);
}
