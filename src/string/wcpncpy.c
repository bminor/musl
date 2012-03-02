#include <wchar.h>

wchar_t *wcpncpy(wchar_t *d, const wchar_t *s, size_t n)
{
	return wcsncpy(d, s, n) + wcsnlen(s, n);
}
