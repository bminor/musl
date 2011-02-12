#include "stdio_impl.h"

size_t __fpending(FILE *f)
{
	return f->wend ? f->wpos - f->wbase : 0;
}
