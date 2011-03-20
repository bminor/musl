#include <unistd.h>
#include "syscall.h"

unsigned alarm(unsigned seconds)
{
	return syscall(SYS_alarm, seconds);
}
