#include "libm.h"
#if LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024
long lrintl(long double x)
{
	return lrint(x);
}
#else
#define type            long double
#define roundit         rintl
#define dtype           long
#define fn              lrintl

#include "lrint.c"
#endif
