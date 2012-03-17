#include "libm.h"

double __lgamma_r(double, int *);

double lgamma(double x)
{
	return __lgamma_r(x, &signgam);
}

// FIXME
//weak_alias(lgamma, gamma);
