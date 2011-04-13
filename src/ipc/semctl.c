#include <sys/sem.h>
#include <stdarg.h>
#include "syscall.h"
#include "ipc.h"

int semctl(int id, int num, int cmd, ...)
{
	long arg;
	va_list ap;
	va_start(ap, cmd);
	arg = va_arg(ap, long);
	va_end(ap);
#ifdef SYS_semctl
	return syscall(SYS_semctl, id, num, cmd | 0x100, arg);
#else
	return syscall(SYS_ipc, IPCOP_semctl, id, num, cmd | 0x100, &arg);
#endif
}
