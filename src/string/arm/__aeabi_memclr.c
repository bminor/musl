#include <string.h>
#include "libc.h"

void __aeabi_memclr(void *dest, size_t n)
{
	memset(dest, 0, n);
}
weak_alias(__aeabi_memclr, __aeabi_memclr4);
weak_alias(__aeabi_memclr, __aeabi_memclr8);
