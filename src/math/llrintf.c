#include <math.h>

/* assumes LLONG_MAX > 2^24, see comments in lrint.c */

long long llrintf(float x)
{
	return rintf(x);
}
