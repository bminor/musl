#include <unistd.h>
#include "syscall.h"

gid_t getgid(void)
{
	return syscall(SYS_getgid);
}
