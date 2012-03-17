#include <math.h>
#include <float.h>

#if LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024
long lroundl(long double x)
{
	return lround(x);
}
#else
#define type            long double
#define roundit         roundl
#define dtype           long
#define DTYPE_MIN       LONG_MIN
#define DTYPE_MAX       LONG_MAX
#define fn              lroundl

#include "lround.c"
#endif
