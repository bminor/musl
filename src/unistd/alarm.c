#include <unistd.h>
#include "syscall.h"

unsigned alarm(unsigned seconds)
{
	return syscall1(__NR_alarm, seconds);
}
