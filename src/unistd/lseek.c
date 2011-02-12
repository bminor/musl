#include <unistd.h>
#include "syscall.h"
#include "libc.h"

off_t lseek(int fd, off_t offset, int whence)
{
	/* optimized away at compiletime */
	if (sizeof(long) == 8)
		return syscall3(__NR_lseek, fd, offset, whence);
	else {
		off_t result;
		return syscall5(__NR__llseek, fd, offset>>32, offset, (long)&result, whence) ? -1 : result;
	}
}

LFS64(lseek);
