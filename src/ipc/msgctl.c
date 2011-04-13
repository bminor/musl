#include <sys/msg.h>
#include "syscall.h"
#include "ipc.h"

int msgctl(int q, int cmd, struct msqid_ds *buf)
{
#ifdef SYS_msgctl
	return syscall(SYS_msgctl, q, cmd | 0x100, buf);
#else
	return syscall(SYS_ipc, IPCOP_msgctl, q, cmd | 0x100, 0, buf, 0);
#endif
}
