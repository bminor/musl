#include <resolv.h>
#include "libc.h"

int __dn_expand(const unsigned char *base, const unsigned char *end, const unsigned char *src, char *dest, int space)
{
	const unsigned char *p = src;
	char *dend = dest + (space > 254 ? 254 : space);
	int len = -1, i, j;
	if (p==end || !*p) return -1;
	/* detect reference loop using an iteration counter */
	for (i=0; i < end-base; i+=2) {
		if (*p & 0xc0) {
			if (p+1==end) return -1;
			j = ((p[0] & 0x3f) << 8) | p[1];
			if (len < 0) len = p+2-src;
			if (j >= end-base) return -1;
			p = base+j;
		} else if (*p) {
			j = *p+1;
			if (j>=end-p || j>dend-dest) return -1;
			while (--j) *dest++ = *++p;
			*dest++ = *++p ? '.' : 0;
		} else {
			if (len < 0) len = p+1-src;
			return len;
		}
	}
	return -1;
}

weak_alias(__dn_expand, dn_expand);
