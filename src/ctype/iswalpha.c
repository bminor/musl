#include <wctype.h>

int iswalpha(wint_t wc)
{
	return (32U|wc)-'a'<26;
}
