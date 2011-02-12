#include "stdio_impl.h"

int getc_unlocked(FILE *f)
{
	return f->rpos < f->rstop ? *f->rpos++ : __uflow(f);
}

weak_alias (getc_unlocked, fgetc_unlocked);
