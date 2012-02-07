#include <wctype.h>

wint_t iswblank_l(wint_t c, locale_t l)
{
	return iswblank(c);
}
