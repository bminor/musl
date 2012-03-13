#include "libm.h"

// FIXME

float complex cacosf(float complex z)
{
	z = casinf(z);
	return cpackf((float)M_PI_2 - crealf(z), -cimagf(z));
}
