/* origin: FreeBSD /usr/src/lib/msun/src/s_scalbnl.c */
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
/*
 * scalbnl (long double x, int n)
 * scalbnl(x,n) returns x* 2**n  computed by  exponent
 * manipulation rather than by actually performing an
 * exponentiation or a multiplication.
 */

#include "libm.h"

#if LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024
long double scalbnl(long double x, int n)
{
	return scalbn(x, n);
}
#elif (LDBL_MANT_DIG == 64 || LDBL_MANT_DIG == 113) && LDBL_MAX_EXP == 16384
static const long double
huge = 0x1p16000L,
tiny = 0x1p-16000L;

long double scalbnl(long double x, int n)
{
	union IEEEl2bits u;
	int k;

	u.e = x;
	k = u.bits.exp;                    /* extract exponent */
	if (k == 0) {                      /* 0 or subnormal x */
		if ((u.bits.manh|u.bits.manl) == 0)  /* +-0 */
			return x;
		u.e *= 0x1p128;
		k = u.bits.exp - 128;
		if (n < -50000)
			return tiny*x;  /*underflow*/
	}
	if (k == 0x7fff)                   /* NaN or Inf */
		return x + x;
	k = k + n;
	if (k >= 0x7fff)
		return huge*copysignl(huge, x);  /* overflow  */
	if (k > 0) {                       /* normal result */
		u.bits.exp = k;
		return u.e;
	}
	if (k <= -128)
		if (n > 50000)  /* in case integer overflow in n+k */
			return huge*copysign(huge, x);  /*overflow*/
		return tiny*copysign(tiny, x);  /*underflow*/
	k += 128;                          /* subnormal result */
	u.bits.exp = k;
	return u.e*0x1p-128;
}
#endif
