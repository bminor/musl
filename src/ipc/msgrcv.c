#include <sys/msg.h>
#include "syscall.h"
#include "ipc.h"
#include "libc.h"

ssize_t msgrcv(int q, void *m, size_t len, long type, int flag)
{
	ssize_t r;
	CANCELPT_BEGIN;
#ifdef __NR_msgrcv
	r = syscall(SYS_msgrcv, q, m, len, type, flag);
#else
	r = syscall(SYS_ipc, IPCOP_msgrcv, q, len, flag, ((long[]){ (long)m, type }));
#endif
	CANCELPT_END;
	return r;
}
