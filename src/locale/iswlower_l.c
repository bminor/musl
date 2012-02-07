#include <wctype.h>

wint_t iswlower_l(wint_t c, locale_t l)
{
	return iswlower(c);
}
