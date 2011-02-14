#include <unistd.h>
#include "syscall.h"
#include "libc.h"

off_t lseek(int fd, off_t offset, int whence)
{
#ifdef __NR__llseek
	off_t result;
	return syscall5(__NR__llseek, fd, offset>>32, offset, (long)&result, whence) ? -1 : result;
#else
	return syscall3(__NR_lseek, fd, offset, whence);
#endif
}

LFS64(lseek);
