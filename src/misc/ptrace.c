#include <sys/ptrace.h>
#include <stdarg.h>
#include <unistd.h>
#include "syscall.h"

long ptrace(int req, ...)
{
	va_list ap;
	pid_t pid;
	void *addr, *data, *addr2;
	va_start(ap, req);
	pid = va_arg(ap, pid_t);
	addr = va_arg(ap, void *);
	data = va_arg(ap, void *);
	addr2 = va_arg(ap, void *);
	va_end(ap);
	return syscall(SYS_ptrace, req, pid, addr, data, addr2);
}
