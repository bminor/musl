#include <time.h>

#include "__time.h"

struct tm *gmtime_r(const time_t *t, struct tm *result)
{
	__time_to_tm(*t, result);
	result->tm_isdst = 0;
	return result;
}
