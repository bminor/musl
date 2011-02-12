#include <unistd.h>
#include <errno.h>
#include "syscall.h"

char *getcwd(char *buf, size_t size)
{
	return syscall2(__NR_getcwd, (long)buf, size) < 0 ? NULL : buf;
}
