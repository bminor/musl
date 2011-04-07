#include <sys/msg.h>
#include "syscall.h"
#include "ipc.h"
#include "libc.h"

int msgsnd(int q, const void *m, size_t len, int flag)
{
	ssize_t r;
	CANCELPT_BEGIN;
#ifdef SYS_msgsnd
	r = syscall(SYS_msgsnd, q, m, len, flag);
#else
	r = syscall(SYS_ipc, IPCOP_msgsnd, q, len, flag, m);
#endif
	CANCELPT_END;
	return r;
}
