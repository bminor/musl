#include <stdlib.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <fcntl.h>

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

char *ptsname(int fd)
{
	static char buf[9 + sizeof(int)*3 + 1];
	char *s = buf+sizeof(buf)-1;
	int pty;
	if (ioctl (fd, TIOCGPTN, &pty))
		return NULL;
	if (pty) for (; pty; pty/=10) *--s = '0' + pty%10;
	else *--s = '0';
	s -= 9;
        s[0] = '/'; s[1] = 'd'; s[2] = 'e'; s[3] = 'v';
	s[4] = '/'; s[5] = 'p'; s[6] = 't'; s[7] = 's'; s[8] = '/';
	return s;
}
