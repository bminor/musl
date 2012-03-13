#include "libm.h"

double ldexp(double x, int n)
{
	return scalbn(x, n);
}
