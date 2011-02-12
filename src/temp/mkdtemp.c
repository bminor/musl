#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <sys/stat.h>
#include "libc.h"

char *mkdtemp(char *template)
{
	for (;;) {
		if (!mktemp(template)) return 0;
		if (!mkdir(template, 0700)) return template;
		if (errno != EEXIST) return 0;
		/* this is safe because mktemp verified
		 * that we have a valid template string */
		strcpy(template+strlen(template)-6, "XXXXXX");
	}
}
