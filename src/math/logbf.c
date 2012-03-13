#include <limits.h>
#include "libm.h"

float logbf(float x) {
	int i = ilogbf(x);

	if (i == FP_ILOGB0)
		return -1.0f/fabsf(x);
	if (i == FP_ILOGBNAN || i == INT_MAX)
		return x * x;
	return i;
}
