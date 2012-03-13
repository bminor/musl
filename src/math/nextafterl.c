/* origin: FreeBSD /usr/src/lib/msun/src/s_nextafterl.c */
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
/* IEEE functions
 *      nextafter(x,y)
 *      return the next machine floating-point number of x in the
 *      direction toward y.
 *   Special cases:
 */

#include "libm.h"

#if LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024
long double nextafterl(long double x, long double y)
{
	return nextafter(x, y);
}
#elif (LDBL_MANT_DIG == 64 || LDBL_MANT_DIG == 113) && LDBL_MAX_EXP == 16384
long double nextafterl(long double x, long double y)
{
	volatile long double t;
	union IEEEl2bits ux, uy;

	ux.e = x;
	uy.e = y;

	if ((ux.bits.exp == 0x7fff && ((ux.bits.manh&~LDBL_NBIT)|ux.bits.manl) != 0) ||
	    (uy.bits.exp == 0x7fff && ((uy.bits.manh&~LDBL_NBIT)|uy.bits.manl) != 0))
		return x+y;  /* x or y is nan */
	if (x == y)
		return y;    /* x=y, return y */
	if (x == 0.0) {
		/* return +-minsubnormal */
		ux.bits.manh = 0;
		ux.bits.manl = 1;
		ux.bits.sign = uy.bits.sign;
		/* raise underflow flag */
		t = ux.e*ux.e;
		if (t == ux.e)
			return t;
		return ux.e;
	}
	if(x > 0.0 ^ x < y) {  /* x -= ulp */
		if (ux.bits.manl == 0) {
			if ((ux.bits.manh&~LDBL_NBIT) == 0)
				ux.bits.exp--;
			ux.bits.manh = (ux.bits.manh - 1) | (ux.bits.manh & LDBL_NBIT);
		}
		ux.bits.manl--;
	} else {               /* x += ulp */
		ux.bits.manl++;
		if (ux.bits.manl == 0) {
			ux.bits.manh = (ux.bits.manh + 1) | (ux.bits.manh & LDBL_NBIT);
			if ((ux.bits.manh&~LDBL_NBIT)==0)
				ux.bits.exp++;
		}
	}
	if (ux.bits.exp == 0x7fff)  /* overflow  */
		return x+x;
	if (ux.bits.exp == 0) {     /* underflow */
		mask_nbit_l(ux);
		/* raise underflow flag */
		t = ux.e * ux.e;
		if (t != ux.e)
			return ux.e;
	}
	return ux.e;
}
#endif
