#include <sys/shm.h>
#include "syscall.h"
#include "ipc.h"

int shmctl(int id, int cmd, struct shmid_ds *buf)
{
#ifdef __NR_shmctl
	return syscall3(__NR_shmctl, id, cmd, (long)buf);
#else
	return syscall4(__NR_ipc, IPCOP_shmctl, id, cmd | IPC_MODERN, (long)buf);
#endif
}
