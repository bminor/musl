#include <sys/msg.h>
#include "syscall.h"
#include "ipc.h"
#include "libc.h"

ssize_t msgsnd(int q, const void *m, size_t len, int flag)
{
	ssize_t r;
	CANCELPT_BEGIN;
#ifdef __NR_msgsnd
	r = syscall(SYS_msgsnd, q, m, len, flag);
#else
	r = syscall(SYS_ipc, IPCOP_msgsnd, q, len, flag, m);
#endif
	CANCELPT_END;
	return r;
}
