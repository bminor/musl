#include <stdint.h>
#include <math.h>

int __fpclassify(double __x)
{
	union {
		double __d;
		__uint64_t __i;
	} __y = { __x };
	int __ee = __y.__i>>52 & 0x7ff;
	if (!__ee) return __y.__i<<1 ? FP_SUBNORMAL : FP_ZERO;
	if (__ee==0x7ff) return __y.__i<<12 ? FP_NAN : FP_INFINITE;
	return FP_NORMAL;
}
