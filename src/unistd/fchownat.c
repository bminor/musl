#include <unistd.h>
#include "syscall.h"

int fchownat(int fd, const char *path, uid_t uid, gid_t gid, int flag)
{
	return syscall5(__NR_fchownat, fd, (long)path, uid, gid, flag);
}
