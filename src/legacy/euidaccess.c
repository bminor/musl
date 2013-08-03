#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "syscall.h"
#include "libc.h"

int euidaccess(const char *filename, int amode)
{
	int ret = __syscall(SYS_faccessat, AT_FDCWD, filename, amode, AT_EACCESS);
	if (ret != -ENOSYS) return __syscall_ret(ret);

	if (getuid() == geteuid() && getgid() == getegid())
		return syscall(SYS_access, filename, amode);

	return __syscall_ret(-ENOSYS);
}

weak_alias(euidaccess, eaccess);
