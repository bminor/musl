#define _GNU_SOURCE
#include <sys/time.h>

int stime(time_t *t)
{
	struct timeval tv = { .tv_sec = *t, .tv_usec = 0 };
	return settimeofday(&tv, (void *)0);
}
