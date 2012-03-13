#include "libm.h"

/* tan(z) = -i tanh(i z) */

double complex ctan(double complex z)
{
	z = ctanh(cpack(-cimag(z), creal(z)));
	return cpack(cimag(z), -creal(z));
}
