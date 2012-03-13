#include "libm.h"

long double complex conjl(long double complex z)
{
	return cpackl(creall(z), -cimagl(z));
}
