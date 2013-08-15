/* origin: FreeBSD /usr/src/lib/msun/src/e_atan2l.c */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunSoft, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 *
 */
/*
 * See comments in atan2.c.
 * Converted to long double by David Schultz <das@FreeBSD.ORG>.
 */

#include "libm.h"

#if LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024
long double atan2l(long double y, long double x)
{
	return atan2(y, x);
}
#elif (LDBL_MANT_DIG == 64 || LDBL_MANT_DIG == 113) && LDBL_MAX_EXP == 16384
#include "__invtrigl.h"

long double atan2l(long double y, long double x)
{
	union IEEEl2bits ux, uy;
	long double z;
	int m;
	uint16_t exptx, expsignx, expty, expsigny;

	if (isnan(x) || isnan(y))
		return x+y;
	if (x == 1)
		return atanl(y);
	uy.e = y;
	expsigny = uy.xbits.expsign;
	expty = expsigny & 0x7fff;
	ux.e = x;
	expsignx = ux.xbits.expsign;
	exptx = expsignx & 0x7fff;
	m = ((expsigny>>15)&1) | ((expsignx>>14)&2);  /* 2*sign(x)+sign(y) */

	if (y == 0) {
		switch(m) {
		case 0:
		case 1: return y;           /* atan(+-0,+anything)=+-0 */
		case 2: return  2*pio2_hi+0x1p-120f; /* atan(+0,-anything) = pi */
		case 3: return -2*pio2_hi-0x1p-120f; /* atan(-0,-anything) =-pi */
		}
	}
	if (x == 0)
		return m&1 ? -pio2_hi-0x1p-120f : pio2_hi+0x1p-120f;
	/* when x is INF */
	if (exptx == 0x7fff) {
		if (expty == 0x7fff) {
			switch(m) {
			case 0: return  pio2_hi*0.5+0x1p-120f; /* atan(+INF,+INF) */
			case 1: return -pio2_hi*0.5-0x1p-120f; /* atan(-INF,+INF) */
			case 2: return  1.5*pio2_hi+0x1p-120f; /* atan(+INF,-INF) */
			case 3: return -1.5*pio2_hi-0x1p-120f; /* atan(-INF,-INF) */
			}
		} else {
			switch(m) {
			case 0: return  0.0;        /* atan(+...,+INF) */
			case 1: return -0.0;        /* atan(-...,+INF) */
			case 2: return  2*pio2_hi+0x1p-120f; /* atan(+...,-INF) */
			case 3: return -2*pio2_hi-0x1p-120f; /* atan(-...,-INF) */
			}
		}
	}
	/* when y is INF */
	if (exptx+120 < expty || expty == 0x7fff)
		return m&1 ? -pio2_hi-0x1p-120f : pio2_hi+0x1p-120f;

	if ((m&2) && expty+120 < exptx) /* |y/x| tiny, x<0 */
		z = 0.0;
	else
		z = atanl(fabsl(y/x));
	switch (m) {
	case 0: return z;               /* atan(+,+) */
	case 1: return -z;              /* atan(-,+) */
	case 2: return 2*pio2_hi-(z-2*pio2_lo); /* atan(+,-) */
	default: /* case 3 */
		return (z-2*pio2_lo)-2*pio2_hi; /* atan(-,-) */
	}
}
#endif
