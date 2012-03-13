#include "libm.h"

#if LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024
long double complex casinl(long double complex z)
{
	return casin(z);
}
#else
// FIXME
long double complex casinl(long double complex z)
{
	long double complex w;
	long double x, y;

	x = creall(z);
	y = cimagl(z);
	w = cpackl(1.0 - (x - y)*(x + y), -2.0*x*y);
	return clogl(cpackl(-y, x) + csqrtl(w));
}
#endif
