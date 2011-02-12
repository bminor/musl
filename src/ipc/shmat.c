#include <sys/shm.h>
#include "syscall.h"
#include "ipc.h"

#ifdef __NR_shmat
void *shmat(int id, const void *addr, int flag)
{
	return syscall3(__NR_shmat, id, (long)addr, flag);
}
#else
void *shmat(int id, const void *addr, int flag)
{
	unsigned long ret;
	ret = syscall5(__NR_ipc, IPCOP_shmat, id, flag, (long)&addr, (long)addr);
	return (ret > -(unsigned long)SHMLBA) ? (void *)ret : (void *)addr;
}
#endif
