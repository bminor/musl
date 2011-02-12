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

size_t mbsrtowcs(wchar_t *ws, const char **src, size_t wn, mbstate_t *st)
{
	unsigned c;
	const unsigned char *s = *src;
	const wchar_t *wsorig = ws;

	if (!st) st = (void *)&c, c = 0;
	else c = *(unsigned *)st;

	if (c) {
		*(unsigned *)st = 0;
		if (!ws) {
			wn = 0;
			goto resume0;
		}
		goto resume;
	}

	if (!ws) for (wn=0;;) {
		if ((unsigned)*s-SA >= SB-SA) {
			while (((unsigned)s&3) && (unsigned)*s-1<0x7f) s++, wn++;
			while (!(( *(uint32_t*)s | *(uint32_t*)s-0x01010101) & 0x80808080)) s+=4, wn+=4;
			while ((unsigned)*s-1<0x7f) s++, wn++;
			if (!*s) return wn;
			if ((unsigned)*s-SA >= SB-SA) goto ilseq2;
		}
		c = bittab[*s++-SA];
		do {
resume0:
			if (OOB(c,*s)) goto ilseq2; s++;
			c <<= 6; if (!(c&(1U<<31))) break;
#ifdef I_FAILED_TO_RTFM_RFC3629
			if ((unsigned)*s++-0x80 >= 0x40) goto ilseq2;
			c <<= 6; if (!(c&(1U<<31))) break;
			if ((unsigned)*s++-0x80 >= 0x40) goto ilseq2;
			c <<= 6; if (!(c&(1U<<31))) break;
#endif
			if ((unsigned)*s++-0x80 >= 0x40) goto ilseq2;
			c <<= 6; if (!(c&(1U<<31))) break;
			if ((unsigned)*s++-0x80 >= 0x40) goto ilseq2;
		} while (0);
		wn++; c = 0;
	}

	while (wn) {
		if ((unsigned)*s-SA >= SB-SA) {
			if (wn >= 7) {
				while (((unsigned)s&3) && (unsigned)*s-1<0x7f) {
					*ws++ = *s++;
					wn--;
				}
				while (wn>=4 && !(( *(uint32_t*)s | *(uint32_t*)s-0x01010101) & 0x80808080)) {
					*ws++ = *s++;
					*ws++ = *s++;
					*ws++ = *s++;
					*ws++ = *s++;
					wn -= 4;
				}
			}
			while (wn && (unsigned)*s-1<0x7f) {
				*ws++ = *s++;
				wn--;
			}
			if (!wn) break;
			if (!*s) {
				*ws = 0;
				*src = 0;
				return ws-wsorig;
			}
			if ((unsigned)*s-SA >= SB-SA) goto ilseq;
		}
		c = bittab[*s++-SA];
		do {
resume:
			if (OOB(c,*s)) goto ilseq;
			c = (c<<6) | *s++-0x80;
			if (!(c&(1U<<31))) break;

#ifdef I_FAILED_TO_RTFM_RFC3629
			if ((unsigned)*s-0x80 >= 0x40) goto ilseq;
			c = (c<<6) | *s++-0x80;
			if (!(c&(1U<<31))) break;

			if ((unsigned)*s-0x80 >= 0x40) goto ilseq;
			c = (c<<6) | *s++-0x80;
			if (!(c&(1U<<31))) break;
#endif

			if ((unsigned)*s-0x80 >= 0x40) goto ilseq;
			c = (c<<6) | *s++-0x80;
			if (!(c&(1U<<31))) break;

			if ((unsigned)*s-0x80 >= 0x40) goto ilseq;
			c = (c<<6) | *s++-0x80;
		} while (0);

		*ws++ = c; wn--; c = 0;
	}
	*src = s;
	return ws-wsorig;
ilseq:
	*src = s;
ilseq2:
	/* enter permanently failing state */
	*(unsigned *)st = FAILSTATE;
	errno = EILSEQ;
	return -1;
}
