#include "libm.h"

float modff(float x, float *iptr)
{
	uint32_t u, mask;
	int e;

	GET_FLOAT_WORD(u, x);
	e = (int)(u>>23 & 0xff) - 0x7f;

	/* no fractional part */
	if (e >= 23) {
		*iptr = x;
		if (e == 0x80 && u<<9 != 0) /* nan */
			return x;
		SET_FLOAT_WORD(x, u & 0x80000000);
		return x;
	}
	/* no integral part */
	if (e < 0) {
		SET_FLOAT_WORD(*iptr, u & 0x80000000);
		return x;
	}

	mask = 0x007fffff>>e;
	if ((u & mask) == 0) {
		*iptr = x;
		SET_FLOAT_WORD(x, u & 0x80000000);
		return x;
	}
	SET_FLOAT_WORD(*iptr, u & ~mask);
	return x - *iptr;
}
