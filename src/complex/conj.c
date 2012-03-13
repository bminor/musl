#include "libm.h"

double complex conj(double complex z)
{
	return cpack(creal(z), -cimag(z));
}
