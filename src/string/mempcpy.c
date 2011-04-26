#include <string.h>

void *mempcpy(void *dest, const void *src, size_t n)
{
	memcpy(dest, src, n);
	return (char *)dest + n;
}
