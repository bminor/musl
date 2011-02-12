#include <unistd.h>
#include "syscall.h"
#include "libc.h"

int truncate(const char *path, off_t length)
{
	if (sizeof(long) == 8)
		return syscall2(__NR_truncate, (long)path, length);
	else {
		union { long long ll; long l[2]; } u = { length };
		return syscall3(__NR_truncate64, (long)path, u.l[0], u.l[1]);
	}
}

LFS64(truncate);
