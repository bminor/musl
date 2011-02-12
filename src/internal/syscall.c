#include <errno.h>
#include <unistd.h>

long __syscall_ret(unsigned long r)
{
	if (r >= (unsigned long)-1 - 4096) {
		errno = -(long)r;
		return -1;
	}
	return (long)r;
}
