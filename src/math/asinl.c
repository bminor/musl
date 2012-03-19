/* origin: FreeBSD /usr/src/lib/msun/src/e_asinl.c */
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
/*
 * See comments in asin.c.
 * Converted to long double by David Schultz <das@FreeBSD.ORG>.
 */

#include "libm.h"

#if LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024
long double asinl(long double x)
{
	return asin(x);
}
#elif (LDBL_MANT_DIG == 64 || LDBL_MANT_DIG == 113) && LDBL_MAX_EXP == 16384
#include "__invtrigl.h"
static const long double huge = 1.000e+300;

long double asinl(long double x)
{
	union IEEEl2bits u;
	long double t=0.0,w,p,q,c,r,s;
	int16_t expsign, expt;

	u.e = x;
	expsign = u.xbits.expsign;
	expt = expsign & 0x7fff;
	if (expt >= BIAS) {          /* |x|>= 1 */
		if (expt == BIAS &&
		    ((u.bits.manh&~LDBL_NBIT)|u.bits.manl) == 0)
			/* asin(1)=+-pi/2 with inexact */
			return x*pio2_hi + x*pio2_lo;
		return (x-x)/(x-x);  /* asin(|x|>1) is NaN */
	} else if (expt < BIAS-1) {  /* |x|<0.5 */
		if (expt < ASIN_LINEAR) {  /* if |x| is small, asinl(x)=x */
			/* return x with inexact if x!=0 */
			if (huge+x > 1.0)
				return x;
		}
		t = x*x;
		p = P(t);
		q = Q(t);
		w = p/q;
		return x + x*w;
	}
	/* 1 > |x| >= 0.5 */
	w = 1.0 - fabsl(x);
	t = w*0.5;
	p = P(t);
	q = Q(t);
	s = sqrtl(t);
	if (u.bits.manh >= THRESH) { /* if |x| is close to 1 */
		w = p/q;
		t = pio2_hi-(2.0*(s+s*w)-pio2_lo);
	} else {
		u.e = s;
		u.bits.manl = 0;
		w = u.e;
		c = (t-w*w)/(s+w);
		r = p/q;
		p = 2.0*s*r-(pio2_lo-2.0*c);
		q = pio4_hi-2.0*w;
		t = pio4_hi-(p-q);
	}
	if (expsign > 0)
		return t;
	return -t;
}
#endif
