#include <unistd.h>
#include "syscall.h"
#include "libc.h"

int close(int fd)
{
	int ret = syscall_cp(SYS_close, fd);
	if (libc.testcancel) libc.testcancel();
	return ret;
}
