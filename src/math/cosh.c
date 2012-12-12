/* origin: FreeBSD /usr/src/lib/msun/src/e_cosh.c */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunSoft, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */
/* cosh(x)
 * Method :
 * mathematically cosh(x) if defined to be (exp(x)+exp(-x))/2
 *      1. Replace x by |x| (cosh(x) = cosh(-x)).
 *      2.
 *                                                      [ exp(x) - 1 ]^2
 *          0        <= x <= ln2/2  :  cosh(x) := 1 + -------------------
 *                                                         2*exp(x)
 *
 *                                                exp(x) +  1/exp(x)
 *          ln2/2    <= x <= 22     :  cosh(x) := -------------------
 *                                                        2
 *          22       <= x <= lnovft :  cosh(x) := exp(x)/2
 *          lnovft   <= x <= ln2ovft:  cosh(x) := exp(x/2)/2 * exp(x/2)
 *          ln2ovft  <  x           :  cosh(x) := inf (overflow)
 *
 * Special cases:
 *      cosh(x) is |x| if x is +INF, -INF, or NaN.
 *      only cosh(0)=1 is exact for finite x.
 */

#include "libm.h"

double cosh(double x)
{
	union {double f; uint64_t i;} u = {.f = x};
	uint32_t ix;
	double t;

	/* |x| */
	u.i &= (uint64_t)-1/2;
	x = u.f;
	ix = u.i >> 32;

	/* |x| in [0,0.5*ln2], return 1+expm1(|x|)^2/(2*exp(|x|)) */
	if (ix < 0x3fd62e43) {
		t = expm1(x);
		if (ix < 0x3c800000)
			return 1;
		return 1 + t*t/(2*(1+t));
	}

	/* |x| in [0.5*ln2,22], return (exp(|x|)+1/exp(|x|))/2; */
	if (ix < 0x40360000) {
		t = exp(x);
		return 0.5*t + 0.5/t;
	}

	/* |x| in [22, log(maxdouble)] return 0.5*exp(|x|) */
	if (ix < 0x40862e42)
		return 0.5*exp(x);

	/* |x| in [log(maxdouble), overflowthresold] */
	if (ix <= 0x408633ce)
		return __expo2(x);

	/* overflow (or nan) */
	x *= 0x1p1023;
	return x;
}
