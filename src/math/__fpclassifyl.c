#include <stdint.h>
#include <math.h>

/* FIXME: move this to arch-specific file */
int __fpclassifyl(long double __x)
{
	union {
		long double __ld;
		__uint16_t __hw[5];
		__int64_t __m;
	} __y = { __x };
	int __ee = __y.__hw[4]&0x7fff;
	if (!__ee) return __y.__m ? FP_SUBNORMAL : FP_ZERO;
	if (__ee==0x7fff) return __y.__m ? FP_NAN : FP_INFINITE;
	return __y.__m < 0 ? FP_NORMAL : FP_NAN;
}
