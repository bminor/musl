#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <stdint.h>
#include "libc.h"

char *__mktemp(char *template)
{
	struct timespec ts;
	size_t l = strlen(template);
	int retries = 10000;
	unsigned long r;

	if (l < 6 || strcmp(template+l-6, "XXXXXX")) {
		errno = EINVAL;
		return 0;
	}
	clock_gettime(CLOCK_REALTIME, &ts);
	r = ts.tv_nsec + (uintptr_t)&ts / 16 + (uintptr_t)template;
	while (retries--) {
		snprintf(template+l-6, 7, "%06lX", r & 0xffffff);
		if (access(template, F_OK) < 0) return template;
		r = r * 1103515245 + 12345;
	}
	*template = 0;
	errno = EEXIST;
	return template;
}

weak_alias(__mktemp, mktemp);
