#include "stdio_impl.h"

int __stdio_close(FILE *f)
{
	return __syscall_close(f->fd);
}
