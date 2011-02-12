#include <sys/sem.h>
#include "syscall.h"
#include "ipc.h"

int semop(int id, struct sembuf *buf, size_t n)
{
#ifdef __NR_semop
	return syscall3(__NR_semop, id, (long)buf, n);
#else
	return syscall5(__NR_ipc, IPCOP_semop, id, n, 0, (long)buf);
#endif
}
