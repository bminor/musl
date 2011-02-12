#include <math.h>
#include <inttypes.h>

/* This version is for 80-bit little endian long double */

long double frexpl(long double x, int *e)
{
	union { long double ld; uint16_t hw[5]; } y = { x };
	int ee = y.hw[4]&0x7fff;

	if (!ee) {
		if (x) {
			x = frexpl(x*0x1p64, e);
			*e -= 64;
		} else *e = 0;
		return x;
	} else if (ee == 0x7fff) {
		return x;
	}

	*e = ee - 0x3ffe;
	y.hw[4] &= 0x8000;
	y.hw[4] |= 0x3ffe;
	return y.ld;
}
