#include "stdio_impl.h"

#undef feof

int feof(FILE *f)
{
	return !!(f->flags & F_EOF);
}

weak_alias(feof, feof_unlocked);
