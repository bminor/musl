#include "stdio_impl.h"

wint_t putwc(wchar_t c, FILE *f)
{
	return fputwc(c, f);
}
