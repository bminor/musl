/* origin: OpenBSD /usr/src/lib/libm/src/ld80/e_coshl.c */
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
/* coshl(x)
 * Method :
 * mathematically coshl(x) if defined to be (exp(x)+exp(-x))/2
 *      1. Replace x by |x| (coshl(x) = coshl(-x)).
 *      2.
 *                                                      [ exp(x) - 1 ]^2
 *          0        <= x <= ln2/2  :  coshl(x) := 1 + -------------------
 *                                                         2*exp(x)
 *
 *                                                 exp(x) +  1/exp(x)
 *          ln2/2    <= x <= 22     :  coshl(x) := -------------------
 *                                                         2
 *          22       <= x <= lnovft :  coshl(x) := expl(x)/2
 *          lnovft   <= x <= ln2ovft:  coshl(x) := expl(x/2)/2 * expl(x/2)
 *          ln2ovft  <  x           :  coshl(x) := inf (overflow)
 *
 * Special cases:
 *      coshl(x) is |x| if x is +INF, -INF, or NaN.
 *      only coshl(0)=1 is exact for finite x.
 */

#include "libm.h"

#if LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024
long double coshl(long double x)
{
	return cosh(x);
}
#elif LDBL_MANT_DIG == 64 && LDBL_MAX_EXP == 16384
long double coshl(long double x)
{
	union {
		long double f;
		struct{uint64_t m; uint16_t se; uint16_t pad;} i;
	} u = {.f = x};
	unsigned ex = u.i.se & 0x7fff;
	long double t;
	uint32_t mx,lx;

	/* |x| */
	u.i.se = ex;
	x = u.f;
	mx = u.i.m >> 32;
	lx = u.i.m;

	/* |x| in [0,0.5*ln2], return 1+expm1l(|x|)^2/(2*expl(|x|)) */
	if (ex < 0x3fff-2 || (ex == 0x3fff-2 && mx < 0xb17217f7)) {
		t = expm1l(x);
		if (ex < 0x3fff-64)
			return 1;
		return 1 + t*t/(2*(1+t));
	}

	/* |x| in [0.5*ln2,22], return (exp(|x|)+1/exp(|x|)/2; */
	if (ex < 0x3fff+4 || (ex == 0x3fff+4 && mx < 0xb0000000)) {
		t = expl(x);
		return 0.5*t + 0.5/t;
	}

	/* |x| in [22, ln(maxdouble)] return 0.5*exp(|x|) */
	if (ex < 0x3fff+13 || (ex == 0x3fff+13 && mx < 0xb1700000))
		return 0.5*expl(x);

	/* |x| in [log(maxdouble), log(2*maxdouble)) */
	if (ex == 0x3fff+13 && (mx < 0xb174ddc0 ||
	     (mx == 0xb174ddc0 && lx < 0x31aec0eb))) {
		t = expl(0.5*x);
		return 0.5*t*t;
	}

	/* |x| >= log(2*maxdouble) or nan */
	return x*0x1p16383L;
}
#endif
