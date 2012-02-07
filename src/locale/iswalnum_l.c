#include <wctype.h>

wint_t iswalnum_l(wint_t c, locale_t l)
{
	return iswalnum(c);
}
