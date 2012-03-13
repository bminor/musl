/* origin: FreeBSD /usr/src/lib/msun/src/s_nextafter.c */
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

double nextafter(double x, double y)
{
	volatile double t;
	int32_t hx,hy,ix,iy;
	uint32_t lx,ly;

	EXTRACT_WORDS(hx, lx, x);
	EXTRACT_WORDS(hy, ly, y);
	ix = hx & 0x7fffffff;  /* |x| */
	iy = hy & 0x7fffffff;  /* |y| */

	if ((ix >= 0x7ff00000 && (ix-0x7ff00000)|lx) != 0 ||   /* x is nan */
	    (iy >= 0x7ff00000 && (iy-0x7ff00000)|ly) != 0)     /* y is nan */
		return x+y;
	if (x == y)          /* x == y */
		return y;
	if ((ix|lx) == 0) {  /* x == 0 */
		INSERT_WORDS(x, hy&0x80000000, 1);  /* return +-minsubnormal */
		/* raise underflow flag */
		t = x*x;
		if (t == x)
			return t;
		return x;
	}
	if (hx >= 0) {  /* x > 0 */
		if (hx > hy || (hx == hy && lx > ly)) {  /* x > y, x -= ulp */
			if (lx == 0)
				hx--;
			lx--;
		} else {                                 /* x < y, x += ulp */
			lx++;
			if (lx == 0)
				hx++;
		}
	} else {        /* x < 0 */
		if (hy >= 0 || hx > hy || (hx == hy && lx > ly)) { /* x < y, x -= ulp */
			if (lx == 0)
				hx--;
			lx--;
		} else {                                 /* x > y, x += ulp */
			lx++;
			if (lx == 0)
				hx++;
		}
	}
	hy = hx & 0x7ff00000;
	if (hy >= 0x7ff00000)  /* overflow  */
		return x+x;
	if (hy < 0x00100000) { /* underflow */
		/* raise underflow flag */
		t = x*x;
		if (t != x) {
			INSERT_WORDS(y, hx, lx);
			return y;
		}
	}
	INSERT_WORDS(x, hx, lx);
	return x;
}
