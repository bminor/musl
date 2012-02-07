#include <wctype.h>

wint_t iswprint_l(wint_t c, locale_t l)
{
	return iswprint(c);
}
