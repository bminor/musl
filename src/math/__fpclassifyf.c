#include <stdint.h>
#include <math.h>

int __fpclassifyf(float __x)
{
	union {
		float __f;
		__uint32_t __i;
	} __y = { __x };
	int __ee = __y.__i>>23 & 0xff;
	if (!__ee) return __y.__i<<1 ? FP_SUBNORMAL : FP_ZERO;
	if (__ee==0xff) return __y.__i<<9 ? FP_NAN : FP_INFINITE;
	return FP_NORMAL;
}
