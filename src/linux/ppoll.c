#define _GNU_SOURCE
#include <poll.h>
#include "syscall.h"

int ppoll(struct pollfd *fds, nfds_t n, const struct timespec *to, const sigset_t *mask)
{
	struct timespec tmp = *to;
	return syscall_cp(SYS_ppoll, fds, n, &tmp, mask);
}
