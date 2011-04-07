#include <unistd.h>
#include "syscall.h"
#include "libc.h"

int setregid(gid_t rgid, gid_t egid)
{
	return __rsyscall(__NR_setregid, rgid, egid, 0, 0, 0, 0);
}
