#include <stdio.h>
#include "syscall.h"

int remove(const char *path)
{
	return syscall(SYS_unlink, path);
}
