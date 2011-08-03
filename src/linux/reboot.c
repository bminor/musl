#include <sys/reboot.h>
#include "syscall.h"

int reboot(int type)
{
	return syscall(SYS_reboot, type);
}
