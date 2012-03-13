#include "libm.h"

/* sin(z) = -i sinh(i z) */

double complex csin(double complex z)
{
	z = csinh(cpack(-cimag(z), creal(z)));
	return cpack(cimag(z), -creal(z));
}
