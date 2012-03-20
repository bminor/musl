/* origin: FreeBSD /usr/src/lib/msun/src/e_acosl.c */
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
 * See comments in acos.c.
 * Converted to long double by David Schultz <das@FreeBSD.ORG>.
 */

#include "libm.h"

#if LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024
long double acosl(long double x)
{
	return acos(x);
}
#elif (LDBL_MANT_DIG == 64 || LDBL_MANT_DIG == 113) && LDBL_MAX_EXP == 16384
#include "__invtrigl.h"

static const long double
pi = 3.14159265358979323846264338327950280e+00L;

long double acosl(long double x)
{
	union IEEEl2bits u;
	long double z, p, q, r, w, s, c, df;
	int16_t expsign, expt;
	u.e = x;
	expsign = u.xbits.expsign;
	expt = expsign & 0x7fff;
	if (expt >= BIAS) {        /* |x| >= 1 */
		if (expt == BIAS &&
			((u.bits.manh & ~LDBL_NBIT) | u.bits.manl) == 0) {
			if (expsign > 0)
				return 0.0;  /* acos(1) = 0 */
			else
				return pi + 2.0 * pio2_lo;  /* acos(-1)= pi */
		}
		return (x - x) / (x - x);  /* acos(|x|>1) is NaN */
	}
	if (expt < BIAS - 1) {     /* |x| < 0.5 */
		if (expt < ACOS_CONST)
			return pio2_hi + pio2_lo;  /* x tiny: acosl=pi/2 */
		z = x * x;
		p = P(z);
		q = Q(z);
		r = p / q;
		return pio2_hi - (x - (pio2_lo - x * r));
	} else if (expsign < 0) {  /* x < -0.5 */
		z = (1.0 + x) * 0.5;
		p = P(z);
		q = Q(z);
		s = sqrtl(z);
		r = p / q;
		w = r * s - pio2_lo;
		return pi - 2.0 * (s + w);
	} else {                   /* x > 0.5 */
		z = (1.0 - x) * 0.5;
		s = sqrtl(z);
		u.e = s;
		u.bits.manl = 0;
		df = u.e;
		c = (z - df * df) / (s + df);
		p = P(z);
		q = Q(z);
		r = p / q;
		w = r * s + c;
		return 2.0 * (df + w);
	}
}
#endif
