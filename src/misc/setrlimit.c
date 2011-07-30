#include <sys/resource.h>
#include <errno.h>
#include "syscall.h"
#include "libc.h"

int __setrlimit(int resource, const struct rlimit *rlim)
{
	long k_rlim[2] = { rlim->rlim_cur, rlim->rlim_max };
	return __syscall(SYS_setrlimit, resource, k_rlim);
}

struct ctx {
	const struct rlimit *rlim;
	int res;
	int err;
};

static void do_setrlimit(void *p)
{
	struct ctx *c = p;
	if (c->err) return;
	c->err = -__setrlimit(c->res, c->rlim);
}

int setrlimit(int resource, const struct rlimit *rlim)
{
	struct ctx c = { .res = resource, .rlim = rlim };
	__synccall(do_setrlimit, &c);
	if (c.err) {
		errno = c.err;
		return -1;
	}
	return 0;
}

LFS64(setrlimit);
