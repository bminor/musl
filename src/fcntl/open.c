#include <fcntl.h>
#include <unistd.h>
#include <stdarg.h>
#include "syscall.h"
#include "libc.h"

int open(const char *filename, int flags, ...)
{
	int r;
	mode_t mode;
	va_list ap;
	va_start(ap, flags);
	mode = va_arg(ap, mode_t);
	va_end(ap);
	CANCELPT_BEGIN;
	r = syscall(SYS_open, filename, flags|O_LARGEFILE, mode);
	CANCELPT_END;
	return r;
}

LFS64(open);
