#include <math.h>

// FIXME: use lanczos approximation

float __lgammaf_r(float, int *);

float tgammaf(float x)
{
	int sign;
	float y;

	y = exp(__lgammaf_r(x, &sign));
	if (sign < 0)
		y = -y;
	return y;
}
