#if 0
#include "stdio_impl.h"

static ssize_t mread(FILE *f, unsigned char *buf, size_t len)
{
	memcpy(buf, 
}

FILE *fmemopen(void *buf, size_t size, const char *mode)
{
	FILE *f = calloc(sizeof(FILE), 1);
	if (!f) return 0;
	
	//
}
#endif
