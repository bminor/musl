#define _GNU_SOURCE
#include <math.h>

void sincosl(long double t, long double *y, long double *x)
{
	*y = sinl(t);
	*x = cosl(t);
}
