#include <sys/reboot.h>
#include <errno.h>

int reboot(int type)
{
	errno = ENOSYS;
	return -1;
}
