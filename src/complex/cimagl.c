#include "libm.h"

long double (cimagl)(long double complex z)
{
	union lcomplex u = {z};
	return u.a[1];
}
