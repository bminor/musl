#include <sys/select.h>
#include "syscall.h"
#include "libc.h"

int select(int n, fd_set *rfds, fd_set *wfds, fd_set *efds, struct timeval *tv)
{
	int r;
	CANCELPT_BEGIN;
	r = syscall5(__NR__newselect, n, (long)rfds, (long)wfds, (long)efds, (long)tv);
	CANCELPT_END;
	return r;
}
