#include "libm.h"

#if LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024
long double complex catanhl(long double complex z)
{
	return catanh(z);
}
#else
long double complex catanhl(long double complex z)
{
	z = catanl(cpackl(-cimagl(z), creall(z)));
	return cpackl(cimagl(z), -creall(z));
}
#endif
