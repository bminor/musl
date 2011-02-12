#include <sys/socket.h>
#include "syscall.h"
#include "socketcall.h"

int setsockopt(int fd, int level, int optname, const void *optval, socklen_t optlen)
{
	unsigned long args[] = { fd, level, optname, (unsigned long)optval, optlen };
	return syscall2(__NR_socketcall, SYS_SETSOCKOPT, (long)args);
}
