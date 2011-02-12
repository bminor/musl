#include <unistd.h>
#include "syscall.h"
#include "libc.h"

int ftruncate(int fd, off_t length)
{
	if (sizeof(long) == 8)
		return syscall2(__NR_ftruncate, fd, length);
	else {
		union { long long ll; long l[2]; } u = { length };
		return syscall3(__NR_ftruncate64, fd, u.l[0], u.l[1]);
	}
}

LFS64(ftruncate);
