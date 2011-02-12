#include <unistd.h>
#include "syscall.h"

int access(const char *filename, int amode)
{
	return syscall2(__NR_access, (long)filename, amode);
}
