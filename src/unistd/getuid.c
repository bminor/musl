#include <unistd.h>
#include "syscall.h"

uid_t getuid(void)
{
	return syscall0(__NR_getuid32);
}
