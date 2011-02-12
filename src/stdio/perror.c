#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "stdio_impl.h"

void perror(const char *msg)
{
#if 1
	if (msg) fprintf(stderr, "%s: %m\n", msg, strerror(errno));
	else fprintf(stderr, "%m\n");
#else
	FILE *f = stderr;
	char *errstr = strerror(errno);

	FLOCK(f);
	
	if (msg) {
		__fwritex(msg, strlen(msg), f);
		__putc_unlocked(':', f);
		__putc_unlocked(' ', f);
	}
	__fwritex(errstr, strlen(errstr), f);
	__putc_unlocked('\n', f);

	FUNLOCK(f);
#endif
}
