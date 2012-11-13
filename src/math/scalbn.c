#include "libm.h"

double scalbn(double x, int n)
{
	/* make sure result is stored as double on overflow or underflow */
	volatile double z;
	double scale;

	if (n > 1023) {
		x *= 0x1p1023;
		n -= 1023;
		if (n > 1023) {
			x *= 0x1p1023;
			n -= 1023;
			if (n > 1023) {
				z = x * 0x1p1023;
				return z;
			}
		}
	} else if (n < -1022) {
		x *= 0x1p-1022;
		n += 1022;
		if (n < -1022) {
			x *= 0x1p-1022;
			n += 1022;
			if (n < -1022) {
				z = x * 0x1p-1022;
				return z;
			}
		}
	}
	INSERT_WORDS(scale, (uint32_t)(0x3ff+n)<<20, 0);
	z = x * scale;
	return z;
}
