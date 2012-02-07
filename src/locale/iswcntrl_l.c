#include <wctype.h>

wint_t iswcntrl_l(wint_t c, locale_t l)
{
	return iswcntrl(c);
}
