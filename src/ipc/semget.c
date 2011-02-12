#include <sys/sem.h>
#include "syscall.h"
#include "ipc.h"

int semget(key_t key, int n, int fl)
{
#ifdef __NR_semget
	return syscall3(__NR_semget, key, n, fl);
#else
	return syscall4(__NR_ipc, IPCOP_semget, key, n, fl);
#endif
}
