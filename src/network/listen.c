#include <sys/socket.h>
#include "syscall.h"
#include "socketcall.h"

int listen(int fd, int backlog)
{
	unsigned long args[] = { fd, backlog };
	return syscall2(__NR_socketcall, SYS_LISTEN, (long)args);
}
