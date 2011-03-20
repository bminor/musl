#include <unistd.h>
#include "syscall.h"

uid_t getuid(void)
{
	return syscall(SYS_getuid);
}
