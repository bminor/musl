#include <stdlib.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <fcntl.h>
#include "libc.h"

int posix_openpt(int flags)
{
	return open("/dev/ptmx", flags);
}

int grantpt(int fd)
{
	return 0;
}

int unlockpt(int fd)
{
	int unlock = 0;
	return ioctl(fd, TIOCSPTLCK, &unlock);
}

int __ptsname_r(int fd, char *buf, size_t len)
{
	int pty;
	if (!buf) len = 0;
	return -( ioctl (fd, TIOCGPTN, &pty) < 0
		|| snprintf(buf, len, "/dev/pts/%d", pty) >= len );
}

weak_alias(__ptsname_r, ptsname_r);
