#include <wctype.h>
#include <string.h>

wctrans_t wctrans(const char *class)
{
	if (!strcmp(class, "toupper")) return 1;
	if (!strcmp(class, "tolower")) return 2;
	return 0;
}

wint_t towctrans(wint_t wc, wctrans_t trans)
{
	if (trans == 1) return towupper(wc);
	if (trans == 2) return towlower(wc);
	return wc;
}
