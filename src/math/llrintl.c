#include "libm.h"
#if LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024
long long llrintl(long double x)
{
	return llrint(x);
}
#else
#define type            long double
#define roundit         rintl
#define dtype           long long
#define fn              llrintl

#include "lrint.c"
#endif
