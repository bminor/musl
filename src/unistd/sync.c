#include <unistd.h>
#include "syscall.h"

void sync(void)
{
	syscall(SYS_sync);
}
