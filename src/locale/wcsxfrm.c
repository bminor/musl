#include <wchar.h>

/* collate only by code points */
size_t wcsxfrm(wchar_t *dest, const wchar_t *src, size_t n)
{
	size_t l = wcslen(src);
	if (l >= n) {
		wmemcpy(dest, src, n-1);
		dest[n-1] = 0;
	} else wcscpy(dest, src);
	return l;
}
