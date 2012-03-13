#include "libm.h"

float complex cacoshf(float complex z)
{
	z = cacosf(z);
	return cpackf(-cimagf(z), crealf(z));
}
