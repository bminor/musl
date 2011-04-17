#include "stdio_impl.h"
#include "pthread_impl.h"

void flockfile(FILE *f)
{
	if (!libc.threaded) pthread_self();
	__lockfile(f);
}
