/* origin: FreeBSD /usr/src/lib/msun/src/e_coshf.c */
/*
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

#include "libm.h"

float coshf(float x)
{
	union {float f; uint32_t i;} u = {.f = x};
	uint32_t ix;
	float t;

	/* |x| */
	u.i &= 0x7fffffff;
	x = u.f;
	ix = u.i;

	/* |x| in [0,0.5*ln2], return 1+expm1(|x|)^2/(2*exp(|x|)) */
	if (ix < 0x3eb17218) {
		t = expm1f(x);
		if (ix < 0x39800000)
			return 1;
		return 1 + t*t/(2*(1+t));
	}

	/* |x| in [0.5*ln2,9], return (exp(|x|)+1/exp(|x|))/2; */
	if (ix < 0x41100000) {
		t = expf(x);
		return 0.5f*t + 0.5f/t;
	}

	/* |x| in [9, log(maxfloat)] return 0.5f*exp(|x|) */
	if (ix < 0x42b17217)
		return 0.5f*expf(x);

	/* |x| in [log(maxfloat), overflowthresold] */
	if (ix <= 0x42b2d4fc)
		return __expo2f(x);

	/* |x| > overflowthresold or nan */
	x *= 0x1p127f;
	return x;
}
