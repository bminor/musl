#include "libm.h"

float complex ctanf(float complex z)
{
	z = ctanhf(cpackf(-cimagf(z), crealf(z)));
	return cpackf(cimagf(z), -crealf(z));
}
