#include <time.h>

char *__asctime(const struct tm *, char *);

char *asctime_r(const struct tm *tm, char *buf)
{
	return __asctime(tm, buf);
}
