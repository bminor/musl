#include <limits.h>
#include "libm.h"
#if LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024
long double logbl(long double x)
{
	return logb(x);
}
#else
long double logbl(long double x)
{
	int i = ilogbl(x);

	if (i == FP_ILOGB0)
		return -1.0/fabsl(x);
	if (i == FP_ILOGBNAN || i == INT_MAX)
		return x * x;
	return i;
}
#endif
