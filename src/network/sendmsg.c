#include <sys/socket.h>
#include <limits.h>
#include "syscall.h"
#include "libc.h"

ssize_t sendmsg(int fd, const struct msghdr *msg, int flags)
{
	ssize_t r;
#if LONG_MAX > INT_MAX
	struct msghdr h;
	if (msg) {
		h = *msg;
		h.__pad1 = h.__pad2 = 0;
		msg = &h;
	}
#endif
	CANCELPT_BEGIN;
	r = socketcall(sendmsg, fd, msg, flags, 0, 0, 0);
	CANCELPT_END;
	return r;
}
