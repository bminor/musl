#include <unistd.h>
#include "syscall.h"
#include "libc.h"

ssize_t sendfile(int out_fd, int in_fd, off_t *ofs, size_t count)
{
	return syscall4(__NR_sendfile, out_fd, in_fd, (long)ofs, count);
}

LFS64(sendfile);
