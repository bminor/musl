#include <wctype.h>

wint_t iswupper_l(wint_t c, locale_t l)
{
	return iswupper(c);
}
