#include "libm.h"

extern int __signgam;
float __lgammaf_r(float, int *);

float lgammaf(float x)
{
	return __lgammaf_r(x, &__signgam);
}

// FIXME
//weak_alias(lgammaf, gammaf);
