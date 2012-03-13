#include <fenv.h>
#include <math.h>

float nearbyintf(float x) {
	fenv_t e;

	fegetenv(&e);
	x = rintf(x);
	fesetenv(&e);
	return x;
}
