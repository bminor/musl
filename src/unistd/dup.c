#include <unistd.h>
#include "syscall.h"

int dup(int fd)
{
	return syscall1(__NR_dup, fd);
}
