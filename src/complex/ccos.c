#include "libm.h"

/* cos(z) = cosh(i z) */

double complex ccos(double complex z)
{
	return ccosh(cpack(-cimag(z), creal(z)));
}
