#include <sys/socket.h>
#include "syscall.h"
#include "socketcall.h"

int getsockopt(int fd, int level, int optname, void *optval, socklen_t *optlen)
{
	unsigned long args[] = {
		fd, level, optname,
		(unsigned long)optval,
		(unsigned long)optlen
	};
	return syscall2(__NR_socketcall, SYS_GETSOCKOPT, (long)args);
}
