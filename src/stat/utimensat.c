#include <sys/stat.h>
#include "syscall.h"

int utimensat(int fd, const char *path, const struct timespec times[2], int flags)
{
	return syscall4(__NR_utimensat, fd, (long)path, (long)times, flags);
}
