#include "libm.h"

// FIXME

/* asin(z) = -i log(i z + sqrt(1 - z*z)) */

double complex casin(double complex z)
{
	double complex w;
	double x, y;

	x = creal(z);
	y = cimag(z);
	w = cpack(1.0 - (x - y)*(x + y), -2.0*x*y);
	return clog(cpack(-y, x) + csqrt(w));
}
