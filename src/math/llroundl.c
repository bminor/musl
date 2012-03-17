#include <math.h>
#include <float.h>

#if LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024
long long llroundl(long double x)
{
	return llround(x);
}
#else
#define type            long double
#define roundit         roundl
#define dtype           long long
#define DTYPE_MIN       LLONG_MIN
#define DTYPE_MAX       LLONG_MAX
#define fn              llroundl

#include "lround.c"
#endif
