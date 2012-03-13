#include "libm.h"

#if LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024
long double complex csinl(long double complex z)
{
	return csin(z);
}
#else
long double complex csinl(long double complex z)
{
	z = csinhl(cpackl(-cimagl(z), creall(z)));
	return cpackl(cimagl(z), -creall(z));
}
#endif
