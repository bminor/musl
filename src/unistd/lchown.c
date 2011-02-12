#include <unistd.h>
#include "syscall.h"

int lchown(const char *path, uid_t uid, gid_t gid)
{
	return syscall3(__NR_lchown32, (long)path, uid, gid);
}
