#include <wchar.h>
#include <wctype.h>

#undef iswalnum

int iswalnum(wint_t wc)
{
	return (unsigned)wc-'0' < 10 || iswalpha(wc);
}
