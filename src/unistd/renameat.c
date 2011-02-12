#include <stdio.h>
#include "syscall.h"

int renameat(int oldfd, const char *old, int newfd, const char *new)
{
	return syscall4(__NR_renameat, oldfd, (long)old, newfd, (long)new);
}
