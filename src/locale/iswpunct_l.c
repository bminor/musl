#include <wctype.h>

wint_t iswpunct_l(wint_t c, locale_t l)
{
	return iswpunct(c);
}
