#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "stdio_impl.h"

FILE *tmpfile(void)
{
	char buf[L_tmpnam], *s;
	int fd;
	FILE *f;
	for (;;) {
		s = tmpnam(buf);
		if (!s) return NULL;
		fd = __syscall_open(s, O_RDWR | O_CREAT | O_EXCL, 0600);
		if (fd >= 0) {
			f = __fdopen(fd, "w+");
			remove(s);
			return f;
		}
	}
}

LFS64(tmpfile);
