#include <unistd.h>
#include "syscall.h"

int chdir(const char *path)
{
	return syscall1(__NR_chdir, (long)path);
}
