/* 
 * This code was written by Rich Felker in 2010; no copyright is claimed.
 * This code is in the public domain. Attribution is appreciated but
 * unnecessary.
 */

#include <stdlib.h>
#include <inttypes.h>
#include <wchar.h>
#include <errno.h>

#include "internal.h"

size_t wcsnrtombs(char *dst, const wchar_t **wcs, size_t wn, size_t n, mbstate_t *st)
{
	size_t l, cnt=0, n2;
	char *s, buf[256];
	const wchar_t *ws = *wcs;

	if (!dst) s = buf, n = sizeof buf;
	else s = dst;

	while ( n && ( (n2=wn)>=n || n2>32 ) ) {
		if (n2>=n) n2=n;
		wn -= n2;
		l = wcsrtombs(s, &ws, n2, 0);
		if (!(l+1)) {
			cnt = l;
			n = 0;
			break;
		}
		if (s != buf) {
			s += l;
			n -= l;
		}
		cnt += l;
	}
	while (n && wn) {
		l = wcrtomb(s, *ws, 0);
		if (!(l+1)) {
			cnt = l;
			break;
		}
		ws++; wn--;
		/* safe - this loop runs fewer than sizeof(buf) times */
		s+=l; n-=l;
		cnt++;
	}
	if (dst) *wcs = ws;
	return cnt;
}
