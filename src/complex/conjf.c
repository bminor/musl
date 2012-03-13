#include "libm.h"

float complex conjf(float complex z)
{
	return cpackf(crealf(z), -cimagf(z));
}
