#include <sys/mman.h>
#include "syscall.h"

int mlockall(int flags)
{
	return syscall1(__NR_mlockall, flags);
}
