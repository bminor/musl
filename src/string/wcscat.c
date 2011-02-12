#include <wchar.h>

wchar_t *wcscat(wchar_t *dest, const wchar_t *src)
{
	wcscpy(dest + wcslen(dest), src);
	return dest;
}
