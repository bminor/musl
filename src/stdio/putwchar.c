#include "stdio_impl.h"

wint_t putwchar(wchar_t c)
{
	return fputwc(c, stdout);
}

weak_alias(putwchar, putwchar_unlocked);
