#include "libc.h"

#ifdef __PIC__
struct __libc *__libc_loc()
{
	static struct __libc __libc;
	return &__libc;
}
#else
struct __libc __libc;
#endif
