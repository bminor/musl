#include <sys/socket.h>
#include "syscall.h"
#include "socketcall.h"
#include "libc.h"

ssize_t send(int fd, const void *buf, size_t len, int flags)
{
	unsigned long args[] = { fd, (unsigned long)buf, len, flags };
	ssize_t r;
	CANCELPT_BEGIN;
	r = syscall2(__NR_socketcall, SYS_SEND, (long)args);
	CANCELPT_END;
	return r;
}
