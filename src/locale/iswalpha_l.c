#include <wctype.h>

wint_t iswalpha_l(wint_t c, locale_t l)
{
	return iswalpha(c);
}
