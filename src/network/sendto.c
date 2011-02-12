#include <sys/socket.h>
#include "syscall.h"
#include "socketcall.h"
#include "libc.h"

ssize_t sendto(int fd, const void *buf, size_t len, int flags, const struct sockaddr *addr, socklen_t alen)
{
	unsigned long args[] = {
		fd, (unsigned long)buf, len, flags,
		(unsigned long)addr, alen
	};
	ssize_t r;
	CANCELPT_BEGIN;
	r = syscall2(__NR_socketcall, SYS_SENDTO, (long)args);
	CANCELPT_END;
	return r;
}
