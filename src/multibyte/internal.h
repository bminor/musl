/* 
 * This code was written by Rich Felker in 2010; no copyright is claimed.
 * This code is in the public domain. Attribution is appreciated but
 * unnecessary.
 */

#define LIBC
#ifndef LIBC
/* rename functions not to conflict with libc */
#ifndef myprefix
#define myprefix fsmu8_
#endif
#define concat2(a,b) a ## b
#define concat(a,b) concat2(a,b)
#define prefix(b) concat(myprefix,b)

#undef mblen
#undef mbrlen
#undef mbrtowc
#undef mbsinit
#undef mbsnrtowcs
#undef mbsrtowcs
#undef wcrtomb
#undef wcsrtombs
#undef wcstombs
#undef wctomb
#define mblen prefix(mblen)
#define mbrlen prefix(mbrlen)
#define mbrtowc prefix(mbrtowc)
#define mbsinit prefix(mbsinit)
#define mbsnrtowcs prefix(mbsnrtowcs)
#define mbsrtowcs prefix(mbsrtowcs)
#define mbstowcs prefix(mbstowcs)
#define wcrtomb prefix(wcrtomb)
#define wcsnrtombs prefix(wcsnrtombs)
#define wcsrtombs prefix(wcsrtombs)
#define wcstombs prefix(wcstombs)
#define wctomb prefix(wctomb)

#define bittab prefix(bittab)
#else
#define bittab __fsmu8
#endif

extern const uint32_t bittab[];

/* Upper 6 state bits are a negative integer offset to bound-check next byte */
/*    equivalent to: ( (b-0x80) | (b+offset) ) & ~0x3f      */
#define OOB(c,b) (((((b)>>3)-0x10)|(((b)>>3)+((int32_t)(c)>>26))) & ~7)

/* Interval [a,b). Either a must be 80 or b must be c0, lower 3 bits clear. */
#define R(a,b) ((uint32_t)((a==0x80 ? 0x40-b : -a) << 23))
#define FAILSTATE R(0x80,0x80)

#ifdef I_FAILED_TO_RTFM_RFC3629
#define SA 0xc2
#define SB 0xfe
#else
#define SA 0xc2
#define SB 0xf5
#endif
