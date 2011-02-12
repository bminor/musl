#include <stdio.h>
#include "syscall.h"

int remove(const char *path)
{
	return __syscall_unlink(path);
}
