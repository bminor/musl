#include "libm.h"

float complex catanhf(float complex z)
{
	z = catanf(cpackf(-cimagf(z), crealf(z)));
	return cpackf(cimagf(z), -crealf(z));
}
