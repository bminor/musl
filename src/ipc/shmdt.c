#include <sys/shm.h>
#include "syscall.h"
#include "ipc.h"

int shmdt(const void *addr)
{
#ifdef __NR_shmdt
	return syscall1(__NR_shmdt, (long)addr);
#else
	return syscall2(__NR_ipc, IPCOP_shmdt, (long)addr);
#endif
}
