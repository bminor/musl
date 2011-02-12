#include <sys/shm.h>
#include "syscall.h"
#include "ipc.h"

int shmget(key_t key, size_t size, int flag)
{
#ifdef __NR_shmget
	return syscall3(__NR_shmget, key, size, flag);
#else
	return syscall4(__NR_ipc, IPCOP_shmget, key, size, flag);
#endif
}
