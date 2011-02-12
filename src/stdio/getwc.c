#include "stdio_impl.h"

wint_t getwc(FILE *f)
{
	return fgetwc(f);
}
