#include <fenv.h>
#include <math.h>

/*
rint may raise inexact (and it should not alter the fenv otherwise)
nearbyint must not raise inexact

(according to ieee754r section 7.9 both functions should raise invalid
when the input is signaling nan, but c99 does not define snan so saving
and restoring the entire fenv should be fine)
*/

double nearbyint(double x) {
	fenv_t e;

	fegetenv(&e);
	x = rint(x);
	fesetenv(&e);
	return x;
}
