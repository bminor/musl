/* origin: OpenBSD /usr/src/lib/libm/src/ld80/s_tanhl.c */
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
/* tanhl(x)
 * Return the Hyperbolic Tangent of x
 *
 * Method :
 *                                      x    -x
 *                                     e  - e
 *      0. tanhl(x) is defined to be -----------
 *                                      x    -x
 *                                     e  + e
 *      1. reduce x to non-negative by tanhl(-x) = -tanhl(x).
 *      2.  0      <= x <= 2**-55 : tanhl(x) := x*(one+x)
 *                                               -t
 *          2**-55 <  x <=  1     : tanhl(x) := -----; t = expm1l(-2x)
 *                                              t + 2
 *                                                    2
 *          1      <= x <=  23.0  : tanhl(x) := 1-  ----- ; t=expm1l(2x)
 *                                                  t + 2
 *          23.0   <  x <= INF    : tanhl(x) := 1.
 *
 * Special cases:
 *      tanhl(NaN) is NaN;
 *      only tanhl(0)=0 is exact for finite argument.
 */

#include "libm.h"

#if LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024
long double tanhl(long double x)
{
	return tanh(x);
}
#elif LDBL_MANT_DIG == 64 && LDBL_MAX_EXP == 16384
static const long double tiny = 1.0e-4900L;

long double tanhl(long double x)
{
	long double t,z;
	int32_t se;
	uint32_t jj0,jj1,ix;

	/* High word of |x|. */
	GET_LDOUBLE_WORDS(se, jj0, jj1, x);
	ix = se & 0x7fff;

	/* x is INF or NaN */
	if (ix == 0x7fff) {
		/* for NaN it's not important which branch: tanhl(NaN) = NaN */
		if (se & 0x8000)
			return 1.0/x-1.0;  /* tanhl(-inf)= -1; */
		return 1.0/x+1.0;          /* tanhl(+inf)= +1 */
	}

	/* |x| < 23 */
	if (ix < 0x4003 || (ix == 0x4003 && jj0 < 0xb8000000u)) {
		if ((ix|jj0|jj1) == 0) /* x == +- 0 */
			return x;
		if (ix < 0x3fc8)       /* |x| < 2**-55 */
			return x*(1.0+tiny);  /* tanh(small) = small */
		if (ix >= 0x3fff) {    /* |x| >= 1  */
			t = expm1l(2.0*fabsl(x));
			z = 1.0 - 2.0/(t+2.0);
		} else {
			t = expm1l(-2.0*fabsl(x));
			z = -t/(t+2.0);
		}
	/* |x| > 23, return +-1 */
	} else {
		z = 1.0 - tiny;  /* raise inexact flag */
	}
	return se & 0x8000 ? -z : z;
}
#endif
