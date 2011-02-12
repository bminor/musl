#include "stdio_impl.h"

int putc_unlocked(int c, FILE *f)
{
	return f->wpos < f->wstop ? (*f->wpos++ = c) : __overflow(f, c);
}

weak_alias(putc_unlocked, fputc_unlocked);
