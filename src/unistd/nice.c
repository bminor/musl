#include <unistd.h>
#include "syscall.h"

int nice(int inc)
{
	return syscall1(__NR_nice, inc);
}
