#include "stdio_impl.h"

int fileno(FILE *f)
{
	return f->fd;
}

weak_alias(fileno, fileno_unlocked);
