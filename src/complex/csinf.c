#include "libm.h"

float complex csinf(float complex z)
{
	z = csinhf(cpackf(-cimagf(z), crealf(z)));
	return cpackf(cimagf(z), -crealf(z));
}
