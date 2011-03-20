#include <poll.h>
#include "syscall.h"
#include "libc.h"

int poll(struct pollfd *fds, nfds_t n, int timeout)
{
	int r;
	CANCELPT_BEGIN;
	r = syscall(SYS_poll, fds, n, timeout);
	CANCELPT_END;
	return r;
}
