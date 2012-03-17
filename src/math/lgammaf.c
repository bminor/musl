#include "libm.h"

float __lgammaf_r(float, int *);

float lgammaf(float x)
{
	return __lgammaf_r(x, &signgam);
}

// FIXME
//weak_alias(lgammaf, gammaf);
