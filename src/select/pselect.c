#include <sys/select.h>
#include "syscall.h"
#include "libc.h"

int pselect(int n, fd_set *rfds, fd_set *wfds, fd_set *efds, const struct timespec *ts, const sigset_t *mask)
{
	int r;
	long data[2] = { (long)mask, 8 };
	struct timespec ts_tmp;
	if (ts) ts_tmp = *ts;
	CANCELPT_BEGIN;
	r = syscall6(__NR_pselect6, n, (long)rfds, (long)wfds, (long)efds, ts ? (long)&ts_tmp : 0, (long)data);
	CANCELPT_END;
	return r;
}
