#include <sys/sem.h>
#include <stdarg.h>
#include "syscall.h"
#include "ipc.h"

struct semun {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};

int semctl(int id, int num, int cmd, ...)
{
	struct semun arg;
	va_list ap;
	va_start(ap, cmd);
	arg = va_arg(ap, struct semun);
	va_end(ap);
#ifdef SYS_semctl
	return syscall(SYS_semctl, id, num, cmd | IPC_64, arg.buf);
#else
	return syscall(SYS_ipc, IPCOP_semctl, id, num, cmd | IPC_64, &arg.buf);
#endif
}
