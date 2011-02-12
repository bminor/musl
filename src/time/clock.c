#include <time.h>
#include <sys/times.h>

/* this function assumes 100 hz linux and corrects for it */
clock_t clock()
{
	struct tms tms;
	return (unsigned long)times(&tms)*10000;
}
