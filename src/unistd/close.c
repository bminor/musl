#include <unistd.h>
#include "syscall.h"
#include "libc.h"

int close(int fd)
{
	int ret = __syscall_close(fd);
	CANCELPT_BEGIN;
	CANCELPT_END;
	return ret;
}
