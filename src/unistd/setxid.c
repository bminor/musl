#include <unistd.h>
#include <errno.h>
#include <sys/resource.h>
#include "syscall.h"
#include "libc.h"

struct ctx {
	int id, eid, sid;
	int nr, rlim, err;
};

/* We jump through hoops to eliminate the possibility of partial failures. */

static void do_setxid(void *p)
{
	struct ctx *c = p;
	if (c->err) return;
	if (c->rlim && c->id >= 0 && c->id != getuid()) {
		struct rlimit inf = { RLIM_INFINITY, RLIM_INFINITY }, old;
		getrlimit(RLIMIT_NPROC, &old);
		if (setrlimit(RLIMIT_NPROC, &inf) && libc.threads_minus_1) {
			c->err = errno;
			return;
		}
		if (__syscall(c->nr, c->id, c->eid, c->sid))
			c->err = errno;
		setrlimit(RLIMIT_NPROC, &old);
		return;
	}
	if (__syscall(c->nr, c->id, c->eid, c->sid))
		c->err = errno;
}

int __setxid(int nr, int id, int eid, int sid)
{
	struct ctx c = { .nr = nr, .id = id, .eid = eid, .sid = sid };
	switch (nr) {
	case SYS_setuid:
	case SYS_setreuid:
	case SYS_setresuid:
		c.rlim = 1;
	}
	__synccall(do_setxid, &c);
	if (c.err) {
		errno = c.err;
		return -1;
	}
	return 0;
}
