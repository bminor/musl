#include <unistd.h>
#include "syscall.h"

gid_t getgid(void)
{
	return syscall0(__NR_getgid32);
}
