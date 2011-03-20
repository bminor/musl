#include <unistd.h>
#include <time.h>
#include "syscall.h"
#include "libc.h"

int nanosleep(const struct timespec *req, struct timespec *rem)
{
	int ret;
	CANCELPT_BEGIN;
	ret = syscall(SYS_nanosleep, req, rem);
	CANCELPT_END;
	return ret;
}
