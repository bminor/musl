#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include "syscall.h"

char *ctermid(char *s)
{
	static char *s2;
	int fd;
	if (!s) {
		if (!s2) s2 = malloc(L_ctermid);
		s = s2;
	}
	fd = open("/dev/tty", O_WRONLY | O_NOCTTY | O_CLOEXEC);
	if (fd < 0)
		return strcpy(s, "");
	if (ttyname_r(fd, s, L_ctermid))
		strcpy(s, "");
	__syscall(SYS_close, fd);
	return s;
}
