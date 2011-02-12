#include <sys/stat.h>
#include "syscall.h"

mode_t umask(mode_t mode)
{
	return syscall1(__NR_umask, mode);
}
