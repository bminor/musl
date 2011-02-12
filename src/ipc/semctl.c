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
#ifdef __NR_semctl
	return syscall4(__NR_semctl, id, num, cmd, arg);
#else
	return syscall5(__NR_ipc, IPCOP_semctl, id, num, cmd | 0x100, (long)&arg);
#endif
}
