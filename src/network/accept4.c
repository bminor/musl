#define _GNU_SOURCE
#include <sys/socket.h>
#include "syscall.h"
#include "libc.h"

int accept4(int fd, struct sockaddr *restrict addr, socklen_t *restrict len, int flg)
{
	return socketcall_cp(accept4, fd, addr, len, flg, 0, 0);
}
