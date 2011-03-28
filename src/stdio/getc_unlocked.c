#include "stdio_impl.h"

int (getc_unlocked)(FILE *f)
{
	return getc_unlocked(f);
}

weak_alias (getc_unlocked, fgetc_unlocked);
