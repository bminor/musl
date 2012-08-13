#define _GNU_SOURCE
#include <math.h>

long double significandl(long double x)
{
	return scalbnl(x, -ilogbl(x));
}
