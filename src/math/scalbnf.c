#include "libm.h"

float scalbnf(float x, int n)
{
	/* make sure result is stored as double on overflow or underflow */
	volatile float z;
	float scale;

	if (n > 127) {
		x *= 0x1p127f;
		n -= 127;
		if (n > 127) {
			x *= 0x1p127f;
			n -= 127;
			if (n > 127) {
				z = x * 0x1p127f;
				return z;
			}
		}
	} else if (n < -126) {
		x *= 0x1p-126f;
		n += 126;
		if (n < -126) {
			x *= 0x1p-126f;
			n += 126;
			if (n < -126) {
				z = x * 0x1p-126f;
				return z;
			}
		}
	}
	SET_FLOAT_WORD(scale, (uint32_t)(0x7f+n)<<23);
	z = x * scale;
	return z;
}
