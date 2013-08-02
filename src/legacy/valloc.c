#define _BSD_SOURCE
#include <stdlib.h>
#include <limits.h>

void *valloc(size_t size)
{
	return memalign(PAGE_SIZE, size);
}
