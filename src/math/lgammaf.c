#define _GNU_SOURCE
#include "libm.h"

float lgammaf(float x)
{
	return lgamma_r(x, &signgam);
}

// FIXME
//weak_alias(lgammaf, gammaf);
