/* origin: FreeBSD /usr/src/lib/msun/src/ld80/invtrig.h */
/*-
 * Copyright (c) 2008 David Schultz <das@FreeBSD.ORG>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "libm.h"

#if LDBL_MANT_DIG == 64 && LDBL_MAX_EXP == 16384

#define BIAS            (LDBL_MAX_EXP - 1)
#define MANH_SIZE       LDBL_MANH_SIZE

/* Constants shared by the long double inverse trig functions. */
#define pS0     __pS0
#define pS1     __pS1
#define pS2     __pS2
#define pS3     __pS3
#define pS4     __pS4
#define pS5     __pS5
#define pS6     __pS6
#define qS1     __qS1
#define qS2     __qS2
#define qS3     __qS3
#define qS4     __qS4
#define qS5     __qS5
#define pi_hi   __pi_hi
#define pi_lo   __pi_lo
#define pio2_hi __pio2_hi
#define pio2_lo __pio2_lo

extern const long double pS0, pS1, pS2, pS3, pS4, pS5, pS6;
extern const long double qS1, qS2, qS3, qS4, qS5;
extern const long double pi_hi, pi_lo, pio2_hi, pio2_lo;

static long double P(long double x)
{
	return x * (pS0 + x * (pS1 + x * (pS2 + x * (pS3 +
		x * (pS4 + x * (pS5 + x * pS6))))));
}

static long double Q(long double x)
{
	return 1.0 + x * (qS1 + x * (qS2 + x * (qS3 + x * (qS4 + x * qS5))));
}

#endif
