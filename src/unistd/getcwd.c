#include <unistd.h>
#include <errno.h>
#include "syscall.h"

char *getcwd(char *buf, size_t size)
{
	return syscall(SYS_getcwd, buf, size) < 0 ? NULL : buf;
}
