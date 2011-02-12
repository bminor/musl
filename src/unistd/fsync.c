#include <unistd.h>
#include "syscall.h"

int fsync(int fd)
{
	//return syscall1(__NR_fsync, fd);
	return 0;
}
