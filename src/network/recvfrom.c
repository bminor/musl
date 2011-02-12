#include <sys/socket.h>
#include "syscall.h"
#include "socketcall.h"
#include "libc.h"

ssize_t recvfrom(int fd, void *buf, size_t len, int flags, struct sockaddr *addr, socklen_t *alen)
{
	unsigned long args[] = {
		fd, (unsigned long)buf, len, flags,
		(unsigned long)addr, (unsigned long)alen
	};
	ssize_t r;
	CANCELPT_BEGIN;
	r = syscall2(__NR_socketcall, SYS_RECVFROM, (long)args);
	CANCELPT_END;
	return r;
}
