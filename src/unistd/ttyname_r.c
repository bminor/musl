#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

int ttyname_r(int fd, char *name, size_t size)
{
	char procname[sizeof "/proc/self/fd/" + 3*sizeof(int) + 2];
	ssize_t l;

	if (!isatty(fd)) return ENOTTY;

	snprintf(procname, sizeof procname, "/proc/self/fd/%d", fd);
	l = readlink(procname, name, size);

	if (l < 0) return errno;
	else if (l == size) return ERANGE;
	else {
		name[l] = 0;
		return 0;
	}
}
