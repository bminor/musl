/* origin: FreeBSD /usr/src/lib/msun/src/s_nexttowardf.c */
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

#if LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024
// FIXME
#elif (LDBL_MANT_DIG == 64 || LDBL_MANT_DIG == 113) && LDBL_MAX_EXP == 16384
#define LDBL_INFNAN_EXP (LDBL_MAX_EXP * 2 - 1)

float nexttowardf(float x, long double y)
{
	union IEEEl2bits uy;
	volatile float t;
	int32_t hx,ix;

	GET_FLOAT_WORD(hx, x);
	ix = hx & 0x7fffffff;  /* |x| */
	uy.e = y;

	if (ix > 0x7f800000 ||
	    (uy.bits.exp == LDBL_INFNAN_EXP &&
	     ((uy.bits.manh&~LDBL_NBIT)|uy.bits.manl) != 0))
		return x + y;  /* x or y is nan */
	if (x == y)
		return (float)y;  /* x=y, return y */
	if (ix == 0) {   /* x == 0 */
		SET_FLOAT_WORD(x, (uy.bits.sign<<31)|1); /* return +-minsubnormal */
		/* raise underflow flag */
		t = x*x;
		if (t == x)
			return t;
		return x;
	}
	if (hx >= 0 ^ x < y)  /* x -= ulp */
		hx--;
	else                  /* x += ulp */
		hx++;
	ix = hx & 0x7f800000;
	if (ix >= 0x7f800000)  /* overflow  */
		return x+x;
	if (ix < 0x00800000) { /* underflow */
		/* raise underflow flag */
		t = x*x;
		if (t != x) {
			SET_FLOAT_WORD(x, hx);
			return x;
		}
	}
	SET_FLOAT_WORD(x, hx);
	return x;
}
#endif
