#define _GNU_SOURCE
#include "stdio_impl.h"

int __fpurge(FILE *f)
{
	f->wpos = f->wbase = f->wend = 0;
	f->rpos = f->rend = 0;
	return 0;
}

weak_alias(__fpurge, fpurge);
