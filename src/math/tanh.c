/* origin: FreeBSD /usr/src/lib/msun/src/s_tanh.c */
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
/* Tanh(x)
 * Return the Hyperbolic Tangent of x
 *
 * Method :
 *                                     x    -x
 *                                    e  - e
 *      0. tanh(x) is defined to be -----------
 *                                     x    -x
 *                                    e  + e
 *      1. reduce x to non-negative by tanh(-x) = -tanh(x).
 *      2.  0      <= x <  2**-28 : tanh(x) := x with inexact if x != 0
 *                                              -t
 *          2**-28 <= x <  1      : tanh(x) := -----; t = expm1(-2x)
 *                                             t + 2
 *                                                   2
 *          1      <= x <  22     : tanh(x) := 1 - -----; t = expm1(2x)
 *                                                 t + 2
 *          22     <= x <= INF    : tanh(x) := 1.
 *
 * Special cases:
 *      tanh(NaN) is NaN;
 *      only tanh(0)=0 is exact for finite argument.
 */

#include "libm.h"

static const double tiny = 1.0e-300, huge = 1.0e300;

double tanh(double x)
{
	double t,z;
	int32_t jx,ix;

	GET_HIGH_WORD(jx, x);
	ix = jx & 0x7fffffff;

	/* x is INF or NaN */
	if (ix >= 0x7ff00000) {
		if (jx >= 0)
			return 1.0f/x + 1.0f;  /* tanh(+-inf)=+-1 */
		else
			return 1.0f/x - 1.0f;  /* tanh(NaN) = NaN */
	}

	if (ix < 0x40360000) {  /* |x| < 22 */
		if (ix < 0x3e300000) {  /* |x| < 2**-28 */
			/* tanh(tiny) = tiny with inexact */
			if (huge+x > 1.0f)
				return x;
		}
		if (ix >= 0x3ff00000) {  /* |x| >= 1  */
			t = expm1(2.0f*fabs(x));
			z = 1.0f - 2.0f/(t+2.0f);
		} else {
			t = expm1(-2.0f*fabs(x));
			z= -t/(t+2.0f);
		}
	} else {  /* |x| >= 22, return +-1 */
		z = 1.0f - tiny;  /* raise inexact */
	}
	return jx >= 0 ? z : -z;
}
