#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "libc.h"

char *mktemp(char *template)
{
	static int lock;
	static int index;
	int l = strlen(template);

	if (l < 6 || strcmp(template+l-6, "XXXXXX")) {
		errno = EINVAL;
		return NULL;
	}
	LOCK(&lock);
	for (; index < 1000000; index++) {
		snprintf(template+l-6, 6, "%06d", index);
		if (access(template, F_OK) != 0) {
			UNLOCK(&lock);
			return template;
		}
	}
	UNLOCK(&lock);
	return NULL;	
}
