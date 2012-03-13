#include "libm.h"
#if LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024
long double nearbyintl(long double x)
{
	return nearbyint(x);
}
#else
#include <fenv.h>
long double nearbyintl(long double x)
{
	fenv_t e;

	fegetenv(&e);
	x = rintl(x);
	fesetenv(&e);
	return x;
}
#endif
