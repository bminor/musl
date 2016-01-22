#include <string.h>
#include "libc.h"

void __aeabi_memmove(void *dest, const void *src, size_t n)
{
	memmove(dest, src, n);
}
weak_alias(__aeabi_memmove, __aeabi_memmove4);
weak_alias(__aeabi_memmove, __aeabi_memmove8);
