#include <sys/io.h>
#include "syscall.h"

#ifdef SYS_ioperm
int ioperm(unsigned long from, unsigned long num, int turn_on)
{
	return syscall(SYS_ioperm, from, num, turn_on);
}
#endif
