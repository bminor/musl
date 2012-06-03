#include "stdio_impl.h"

#undef ferror

int ferror(FILE *f)
{
	return !!(f->flags & F_ERR);
}

weak_alias(ferror, ferror_unlocked);
weak_alias(ferror, _IO_ferror_unlocked);
