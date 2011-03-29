#include <stdio.h>
#include <errno.h>
#include "syscall.h"

int remove(const char *path)
{
	return (syscall(SYS_unlink, path) && errno == EISDIR)
		? syscall(SYS_rmdir, path) : 0;
}
