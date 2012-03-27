#include <math.h>

// FIXME: use lanczos approximation

double __lgamma_r(double, int *);

double tgamma(double x)
{
	int sign;
	double y;

	y = exp(__lgamma_r(x, &sign));
	if (sign < 0)
		y = -y;
	return y;
}
