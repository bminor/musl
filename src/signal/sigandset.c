#define _GNU_SOURCE
#include <signal.h>
#include <errno.h>

#define SST_SIZE (sizeof(sigset_t) / sizeof(long))

int sigandset(sigset_t *dest, sigset_t *left, sigset_t *right)
{
	if(!dest || !left || !right) {
		errno = EINVAL;
		return -1;
	}
	unsigned long i = 0, *d = (void*) dest, *l = (void*) left, *r = (void*) right;
	for(; i < SST_SIZE; i++) d[i] = l[i] & r[i];
	return 0;
}

