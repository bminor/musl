#include "libc.h"

#ifdef USE_LIBC_ACCESSOR
struct __libc *__libc_loc()
{
	static struct __libc __libc;
	return &__libc;
}
#else
struct __libc __libc ATTR_LIBC_VISIBILITY;
#endif
