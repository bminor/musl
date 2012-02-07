#include <wctype.h>

wint_t iswdigit_l(wint_t c, locale_t l)
{
	return iswdigit(c);
}
