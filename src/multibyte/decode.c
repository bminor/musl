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

/* Decodes UTF-8 byte-by-byte. The c argument must be initialized to 0
 * to begin decoding; when finished it will contain the Unicode scalar
 * value decoded. Return value is 1 if finished, 0 if in-progress, and
 * -1 if an invalid sequence was encountered. After an invalid sequence,
 * the state (in c) automatically resets to 0 if a continuation byte was
 * expected to facilitate a calling idiom of immediately retrying a
 * failed decode call after processing the invalid sequence. If the
 * second try fails, the byte is invalid as a starter as well.
 *
 * A trivial usage idiom is:
 *       while (src<end && (n=decode(dst, *src))>=0) 1[dst+=n]=0, src++;
 */

int decode(unsigned *c, unsigned b)
{
	if (!*c) {
		if (b < 0x80) {
			*c = b;
			return 1;
		} else if (b-SA >= SB-SA) {
			*c = FAILSTATE;
			return -1;
		}
		*c = bittab[b-SA];
		return 0;
	}

	if (OOB(*c,b)) {
		*c = 0;
		return -1;
	}
	*c = *c<<6 | b-0x80;
	return !(*c&(1U<<31));
}
