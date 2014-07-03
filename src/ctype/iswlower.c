#include <wctype.h>
#include "libc.h"

int iswlower(wint_t wc)
{
	return towupper(wc) != wc || wc == 0xdf;
}

int __iswlower_l(wint_t c, locale_t l)
{
	return iswlower(c);
}

weak_alias(__iswlower_l, iswlower_l);
