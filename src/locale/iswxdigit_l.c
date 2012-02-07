#include <wctype.h>

wint_t iswxdigit_l(wint_t c, locale_t l)
{
	return iswxdigit(c);
}
