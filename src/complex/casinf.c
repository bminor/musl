#include "libm.h"

// FIXME

float complex casinf(float complex z)
{
	float complex w;
	float x, y;

	x = crealf(z);
	y = cimagf(z);
	w = cpackf(1.0 - (x - y)*(x + y), -2.0*x*y);
	return clogf(cpackf(-y, x) + csqrtf(w));
}
