#include <unistd.h>
#include "syscall.h"

int faccessat(int fd, const char *filename, int amode, int flag)
{
	return syscall4(__NR_faccessat, fd, (long)filename, amode, flag);
}
