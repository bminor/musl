#include <sys/select.h>
#include "syscall.h"
#include "libc.h"

int select(int n, fd_set *rfds, fd_set *wfds, fd_set *efds, struct timeval *tv)
{
	int r;
	CANCELPT_BEGIN;
	r = syscall(SYS_select, n, rfds, wfds, efds, tv);
	CANCELPT_TRY;
	CANCELPT_END;
	return r;
}
