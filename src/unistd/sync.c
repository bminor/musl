#include <unistd.h>
#include "syscall.h"

void sync(void)
{
	syscall0(__NR_sync);
}
