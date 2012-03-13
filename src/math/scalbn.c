/* origin: FreeBSD /usr/src/lib/msun/src/s_scalbn.c */
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
 * scalbn (double x, int n)
 * scalbn(x,n) returns x* 2**n  computed by  exponent
 * manipulation rather than by actually performing an
 * exponentiation or a multiplication.
 */

#include "libm.h"

static const double
two54  = 1.80143985094819840000e+16, /* 0x43500000, 0x00000000 */
twom54 = 5.55111512312578270212e-17, /* 0x3C900000, 0x00000000 */
huge   = 1.0e+300,
tiny   = 1.0e-300;

double scalbn(double x, int n)
{
// FIXME: k+n check depends on signed int overflow.. use unsigned hx
// TODO: when long != int:
//       scalbln(x,long n) { if(n>9999)n=9999; else if(n<-9999)n=-9999; return scalbn(x,n); }
// TODO: n < -50000 ...
	int32_t k,hx,lx;

	EXTRACT_WORDS(hx, lx, x);
	k = (hx&0x7ff00000)>>20;                /* extract exponent */
	if (k == 0) {                           /* 0 or subnormal x */
		if ((lx|(hx&0x7fffffff)) == 0)  /* +-0 */
			return x;
		x *= two54;
		GET_HIGH_WORD(hx, x);
		k = ((hx&0x7ff00000)>>20) - 54;
		if (n < -50000)
			return tiny*x;          /*underflow*/
	}
	if (k == 0x7ff)                         /* NaN or Inf */
		return x + x;
	k = k + n;
	if (k > 0x7fe)
		return huge*copysign(huge, x);  /* overflow  */
	if (k > 0) {                            /* normal result */
		SET_HIGH_WORD(x, (hx&0x800fffff)|(k<<20));
		return x;
	}
	if (k <= -54)
		if (n > 50000)      /* in case integer overflow in n+k */
			return huge*copysign(huge, x);  /*overflow*/
		return tiny*copysign(tiny, x);  /*underflow*/
	k += 54;                                /* subnormal result */
	SET_HIGH_WORD(x, (hx&0x800fffff)|(k<<20));
	return x*twom54;
}
