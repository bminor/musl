#include "libm.h"

float (cimagf)(float complex z)
{
	union fcomplex u = {z};
	return u.a[1];
}
