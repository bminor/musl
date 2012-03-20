/* origin: FreeBSD /usr/src/lib/msun/src/s_tanhf.c */
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

static const float
tiny = 1.0e-30,
huge = 1.0e30;

float tanhf(float x)
{
	float t,z;
	int32_t jx,ix;

	GET_FLOAT_WORD(jx, x);
	ix = jx & 0x7fffffff;

	/* x is INF or NaN */
	if(ix >= 0x7f800000) {
		if (jx >= 0)
			return 1.0f/x + 1.0f;  /* tanh(+-inf)=+-1 */
		else
			return 1.0f/x - 1.0f;  /* tanh(NaN) = NaN */
	}

	if (ix < 0x41100000) {  /* |x| < 9 */
		if (ix < 0x39800000) {  /* |x| < 2**-12 */
			/* tanh(tiny) = tiny with inexact */
			if (huge+x > 1.0f)
				return x;
		}
		if (ix >= 0x3f800000) {  /* |x|>=1  */
			t = expm1f(2.0f*fabsf(x));
			z = 1.0f - 2.0f/(t+2.0f);
		} else {
			t = expm1f(-2.0f*fabsf(x));
			z = -t/(t+2.0f);
		}
	} else {  /* |x| >= 9, return +-1 */
		z = 1.0f - tiny;  /* raise inexact */
	}
	return jx >= 0 ? z : -z;
}
