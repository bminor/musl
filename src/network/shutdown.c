#include <sys/socket.h>
#include "syscall.h"
#include "socketcall.h"

int shutdown(int fd, int how)
{
	unsigned long args[] = { fd, how };
	return syscall2(__NR_socketcall, SYS_SHUTDOWN, (long)args);
}
