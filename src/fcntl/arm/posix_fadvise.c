#include <fcntl.h>
#include "syscall.h"
#include "libc.h"

int posix_fadvise(int fd, off_t base, off_t len, int advice)
{
	/* ARM-specific syscall argument order */
	return -__syscall(SYS_fadvise, fd, advice,
		__SYSCALL_LL_E(base), __SYSCALL_LL_E(len));
}

LFS64(posix_fadvise);
