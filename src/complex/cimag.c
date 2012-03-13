#include "libm.h"

double (cimag)(double complex z)
{
	union dcomplex u = {z};
	return u.a[1];
}
