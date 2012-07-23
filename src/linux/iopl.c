#include <sys/io.h>
#include "syscall.h"

#ifdef SYS_iopl
int iopl(int level)
{
	return syscall(SYS_iopl, level);
}
#endif
