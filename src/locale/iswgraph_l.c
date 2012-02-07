#include <wctype.h>

wint_t iswgraph_l(wint_t c, locale_t l)
{
	return iswgraph(c);
}
