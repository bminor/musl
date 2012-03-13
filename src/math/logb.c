#include <limits.h>
#include "libm.h"

/*
special cases:
	logb(+-0) = -inf
	logb(+-inf) = +inf
	logb(nan) = nan
these are calculated at runtime to raise fp exceptions
*/

double logb(double x) {
	int i = ilogb(x);

	if (i == FP_ILOGB0)
		return -1.0/fabs(x);
	if (i == FP_ILOGBNAN || i == INT_MAX)
		return x * x;
	return i;
}
