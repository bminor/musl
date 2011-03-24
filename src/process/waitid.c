#include <sys/wait.h>
#include "syscall.h"
#include "libc.h"

int waitid(idtype_t type, id_t id, siginfo_t *info, int options)
{
	int r;
	CANCELPT_BEGIN;
	r = syscall(SYS_waitid, type, id, info, options, 0);
	if (r<0) CANCELPT_TRY;
	CANCELPT_END;
	return r;
}
