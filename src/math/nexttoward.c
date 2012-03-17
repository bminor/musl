/* origin: FreeBSD /usr/src/lib/msun/src/s_nexttoward.c */
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
double nexttoward(double x, long double y)
{
	return nextafter(x, y);
}
#elif (LDBL_MANT_DIG == 64 || LDBL_MANT_DIG == 113) && LDBL_MAX_EXP == 16384
double nexttoward(double x, long double y)
{
	union IEEEl2bits uy;
	volatile double t;
	int32_t hx,ix;
	uint32_t lx;

	EXTRACT_WORDS(hx, lx, x);
	ix = hx & 0x7fffffff;
	uy.e = y;

	if ((ix >= 0x7ff00000 && ((ix-0x7ff00000)|lx) != 0) ||
	    (uy.bits.exp == 0x7fff && ((uy.bits.manh&~LDBL_NBIT)|uy.bits.manl) != 0))
		return x + y;  /* x or y is nan */
	if (x == y)
		return (double)y;
	if (x == 0.0) {
		INSERT_WORDS(x, uy.bits.sign<<31, 1);  /* return +-minsubnormal */
		/* raise underflow */
		t = x * x;
		if (t == x)
			return t;
		return x;
	}
	if (hx > 0.0 ^ x < y) {  /* x -= ulp */
		if (lx == 0)
			hx--;
		lx--;
	} else {                 /* x += ulp */
		lx++;
		if (lx == 0)
			hx++;
	}
	ix = hx & 0x7ff00000;
	if (ix >= 0x7ff00000)   /* overflow  */
		return x + x;
	if (ix < 0x00100000) {  /* underflow */
		/* raise underflow flag */
		t = x * x;
		if (t != x) {
			INSERT_WORDS(x, hx, lx);
			return x;
		}
	}
	INSERT_WORDS(x, hx, lx);
	return x;
}
#endif
