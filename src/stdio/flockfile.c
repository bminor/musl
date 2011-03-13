#include "stdio_impl.h"
#include "pthread_impl.h"

void flockfile(FILE *f)
{
	pthread_self();
	libc.lockfile = __lockfile;
	__lockfile(f);
}
