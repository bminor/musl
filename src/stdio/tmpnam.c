#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include "libc.h"

char *tmpnam(char *s)
{
	static int lock;
	static int index;
	static char *s2;
	int pid = getpid();
	char *dir = getenv("TMPDIR");

	if (!s) {
		if (!s2) s2 = malloc(L_tmpnam);
		s = s2;
	}

	/* this interface is insecure anyway but at least we can try.. */
	if (!dir || strlen(dir) > L_tmpnam-32)
		dir = P_tmpdir;

	if (access(dir, R_OK|W_OK|X_OK) != 0)
		return NULL;

	LOCK(&lock);
	for (index++; index < TMP_MAX; index++) {
		snprintf(s, L_tmpnam, "%s/temp%d-%d", dir, pid, index);
		if (access(s, F_OK) != 0) {
			UNLOCK(&lock);
			return s;
		}
	}
	UNLOCK(&lock);
	return NULL;
}
