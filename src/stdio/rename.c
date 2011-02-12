#include <stdio.h>
#include "syscall.h"

int rename(const char *old, const char *new)
{
	return syscall2(__NR_rename, (long)old, (long)new);
}
