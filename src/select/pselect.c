#include <sys/select.h>
#include "syscall.h"
#include "libc.h"

int pselect(int n, fd_set *rfds, fd_set *wfds, fd_set *efds, const struct timespec *ts, const sigset_t *mask)
{
	long data[2] = { (long)mask, __SYSCALL_SSLEN };
	struct timespec ts_tmp;
	if (ts) ts_tmp = *ts;
	return syscall_cp(SYS_pselect6, n, rfds, wfds, efds, ts ? &ts_tmp : 0, data);
}
