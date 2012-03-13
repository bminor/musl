#include "libm.h"

/* asinh(z) = -i asin(i z) */

double complex casinh(double complex z)
{
	z = casin(cpack(-cimag(z), creal(z)));
	return cpack(cimag(z), -creal(z));
}
