#define _BSD_SOURCE
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "libc.h"

char *__randname(char *);

int __mkostemps(char *template, int len, int flags)
{
	if (len < 0) return EINVAL;

	size_t l = strlen(template)-len;
	if (l < 6 || strncmp(template+l-6, "XXXXXX", 6)) {
		errno = EINVAL;
		*template = 0;
		return -1;
	}

	int fd, retries = 100;
	while (retries--) {
		__randname(template+l-6);
		if ((fd = open(template, flags | O_RDWR | O_CREAT | O_EXCL, 0600))>=0)
			return fd;
		if (errno != EEXIST) return -1;
	}
	return -1;
}

weak_alias(__mkostemps, mkostemps);
weak_alias(__mkostemps, mkostemps64);
