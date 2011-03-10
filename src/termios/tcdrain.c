#include <termios.h>
#include <sys/ioctl.h>
#include "libc.h"

int tcdrain(int fd)
{
	int ret;
	CANCELPT_BEGIN;
	ret = ioctl(fd, TCSBRK, 1);
	CANCELPT_END;
	return ret;
}
