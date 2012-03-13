#include "libm.h"

float complex casinhf(float complex z)
{
	z = casinf(cpackf(-cimagf(z), crealf(z)));
	return cpackf(cimagf(z), -crealf(z));
}
