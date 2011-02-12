#include <unistd.h>
#include "syscall.h"

int chown(const char *path, uid_t uid, gid_t gid)
{
	return syscall3(__NR_chown32, (long)path, uid, gid);
}
