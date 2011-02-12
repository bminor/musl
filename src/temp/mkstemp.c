#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include "libc.h"

int mkstemp(char *template)
{
	int fd;
retry:
	if (!mktemp(template)) return -1;
	fd = open(template, O_RDWR | O_CREAT | O_EXCL, 0600);
	if (fd >= 0) return fd;
	if (errno == EEXIST) {
		/* this is safe because mktemp verified
		 * that we have a valid template string */
		strcpy(template+strlen(template)-6, "XXXXXX");
		goto retry;
	}
	return -1;
}

LFS64(mkstemp);
