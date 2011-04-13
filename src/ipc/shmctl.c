#include <sys/shm.h>
#include "syscall.h"
#include "ipc.h"

int shmctl(int id, int cmd, struct shmid_ds *buf)
{
#ifdef SYS_shmctl
	return syscall(SYS_shmctl, id, cmd | IPC_MODERN, buf);
#else
	return syscall(SYS_ipc, IPCOP_shmctl, id, cmd | IPC_MODERN, 0, buf, 0);
#endif
}
