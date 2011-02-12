#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include "libc.h"

char *tempnam(const char *dir, const char *pfx)
{
	static int lock;
	static int index;
	char *s;
	int pid = getpid();
	int l;

	if (!dir) dir = P_tmpdir;
	if (!pfx) pfx = "temp";

	if (access(dir, R_OK|W_OK|X_OK) != 0)
		return NULL;

	l = strlen(dir) + 1 + strlen(pfx) + 2 + sizeof(int)*3*2 + 1;
	s = malloc(l);
	if (!s) {
		errno = ENOMEM;
		return NULL;
	}

	LOCK(&lock);
	for (; index < TMP_MAX; index++) {
		snprintf(s, l, "%s/%s-%d-%d", dir, pfx, pid, index);
		if (access(s, F_OK) != 0) {
			UNLOCK(&lock);
			return s;
		}
	}
	UNLOCK(&lock);
	free(s);
	return NULL;	
}
