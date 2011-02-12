#include <time.h>

#include "__time.h"

struct tm *localtime_r(const time_t *t, struct tm *result)
{
	__tzset();
	__time_to_tm(*t - __timezone, result);
	result->tm_isdst = -1;
	return __dst_adjust(result);
}
