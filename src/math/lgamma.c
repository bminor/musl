#define _GNU_SOURCE
#include "libm.h"

double lgamma(double x)
{
	return lgamma_r(x, &signgam);
}

// FIXME
//weak_alias(lgamma, gamma);
