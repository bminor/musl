#include <unistd.h>
#include "syscall.h"

gid_t getegid(void)
{
	return syscall0(__NR_getegid32);
}
