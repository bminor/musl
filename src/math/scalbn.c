#include "libm.h"

double scalbn(double x, int n)
{
	double scale;

	if (n > 1023) {
		x *= 0x1p1023;
		n -= 1023;
		if (n > 1023) {
			x *= 0x1p1023;
			n -= 1023;
			if (n > 1023)
				n = 1023;
		}
	} else if (n < -1022) {
		x *= 0x1p-1022;
		n += 1022;
		if (n < -1022) {
			x *= 0x1p-1022;
			n += 1022;
			if (n < -1022)
				n = -1022;
		}
	}
	INSERT_WORDS(scale, (uint32_t)(0x3ff+n)<<20, 0);
	STRICT_ASSIGN(double, x, x * scale);
	return x;
}
