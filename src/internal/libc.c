#include "libc.h"

#ifdef USE_LIBC_ACCESSOR
struct __libc *__libc_loc()
{
	static struct __libc __libc;
	return &__libc;
}
#else
struct __libc __libc;
#endif

#ifdef BROKEN_VISIBILITY
__asm__(".hidden __libc");
#endif

size_t __hwcap;
