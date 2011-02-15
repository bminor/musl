#include <sys/socket.h>
#include "syscall.h"
#include "libc.h"

ssize_t recv(int fd, void *buf, size_t len, int flags)
{
	return recvfrom(fd, buf, len, flags, 0, 0);
}
