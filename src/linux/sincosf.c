#define _GNU_SOURCE
#include <math.h>

void sincosf(float t, float *y, float *x)
{
	*y = sinf(t);
	*x = cosf(t);
}
