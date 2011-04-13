#include <stdlib.h>

int __ptsname_r(int, char *, size_t);

char *ptsname(int fd)
{
	static char buf[9 + sizeof(int)*3 + 1];
	return __ptsname_r(fd, buf, sizeof buf) < 0 ? 0 : buf;
}
