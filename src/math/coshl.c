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
 *          ln2ovft  <  x           :  coshl(x) := huge*huge (overflow)
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
static const long double huge = 1.0e4900L;

long double coshl(long double x)
{
	long double t,w;
	int32_t ex;
	uint32_t mx,lx;

	/* High word of |x|. */
	GET_LDOUBLE_WORDS(ex, mx, lx, x);
	ex &= 0x7fff;

	/* x is INF or NaN */
	if (ex == 0x7fff) return x*x;

	/* |x| in [0,0.5*ln2], return 1+expm1l(|x|)^2/(2*expl(|x|)) */
	if (ex < 0x3ffd || (ex == 0x3ffd && mx < 0xb17217f7u)) {
		t = expm1l(fabsl(x));
		w = 1.0 + t;
		if (ex < 0x3fbc) return w;    /* cosh(tiny) = 1 */
		return 1.0+(t*t)/(w+w);
	}

	/* |x| in [0.5*ln2,22], return (exp(|x|)+1/exp(|x|)/2; */
	if (ex < 0x4003 || (ex == 0x4003 && mx < 0xb0000000u)) {
		t = expl(fabsl(x));
		return 0.5*t + 0.5/t;
	}

	/* |x| in [22, ln(maxdouble)] return 0.5*exp(|x|) */
	if (ex < 0x400c || (ex == 0x400c && mx < 0xb1700000u))
		return 0.5*expl(fabsl(x));

	/* |x| in [log(maxdouble), log(2*maxdouble)) */
	if (ex == 0x400c && (mx < 0xb174ddc0u ||
	     (mx == 0xb174ddc0u && lx < 0x31aec0ebu)))
	{
		w = expl(0.5*fabsl(x));
		t = 0.5*w;
		return t*w;
	}

	/* |x| >= log(2*maxdouble), cosh(x) overflow */
	return huge*huge;
}
#endif
