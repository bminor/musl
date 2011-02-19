#define _GNU_SOURCE
#include <math.h>

void sincos(double t, double *y, double *x)
{
	*y = sin(t);
	*x = cos(t);
}
