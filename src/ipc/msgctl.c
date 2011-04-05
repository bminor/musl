#include <sys/msg.h>
#include "syscall.h"
#include "ipc.h"

int msgctl(int q, int cmd, struct msqid_ds *buf)
{
#ifdef __NR_msgctl
	return syscall(SYS_msgctl, q, cmd, buf);
#else
	return syscall(SYS_ipc, IPCOP_msgctl, q, cmd, buf);
#endif
}
