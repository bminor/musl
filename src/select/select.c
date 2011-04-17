#include <sys/select.h>
#include "syscall.h"
#include "libc.h"

int select(int n, fd_set *rfds, fd_set *wfds, fd_set *efds, struct timeval *tv)
{
	return syscall_cp(SYS_select, n, rfds, wfds, efds, tv);
}
