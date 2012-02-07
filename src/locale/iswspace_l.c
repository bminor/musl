#include <wctype.h>

wint_t iswspace_l(wint_t c, locale_t l)
{
	return iswspace(c);
}
