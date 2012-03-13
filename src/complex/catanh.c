#include "libm.h"

/* atanh = -i atan(i z) */

double complex catanh(double complex z)
{
	z = catan(cpack(-cimag(z), creal(z)));
	return cpack(cimag(z), -creal(z));
}
