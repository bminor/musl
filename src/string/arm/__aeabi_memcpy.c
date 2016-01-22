#include <string.h>
#include "libc.h"

void __aeabi_memcpy(void *restrict dest, const void *restrict src, size_t n)
{
	memcpy(dest, src, n);
}
weak_alias(__aeabi_memcpy, __aeabi_memcpy4);
weak_alias(__aeabi_memcpy, __aeabi_memcpy8);
