#include <sys/ptrace.h>
#include <stdarg.h>
#include <unistd.h>
#include "syscall.h"

long ptrace(int req, ...)
{
	va_list ap;
	pid_t pid;
	void *addr, *data, *addr2;
	long ret, result;

	va_start(ap, req);
	pid = va_arg(ap, pid_t);
	addr = va_arg(ap, void *);
	data = va_arg(ap, void *);
	addr2 = va_arg(ap, void *);
	va_end(ap);

	if (req<4U) data = &result;
	ret = syscall(SYS_ptrace, req, pid, addr, data, addr2);

	if (ret<0 || req>=4U) return ret;
	return result;
}
