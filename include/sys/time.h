#ifndef _SYS_TIME_H
#define _SYS_TIME_H
#ifdef __cplusplus
extern "C" {
#endif

/* All symbols from select.h except pselect are required anyway... */
#include <sys/select.h>

#define __NEED_time_t
#define __NEED_suseconds_t
#define __NEED_timeval

#include <bits/alltypes.h>



int gettimeofday (struct timeval *, void *);

/* extensions */
int settimeofday (const struct timeval *, void *);
int adjtime (const struct timeval *, struct timeval *);


#define ITIMER_REAL    0
#define ITIMER_VIRTUAL 1
#define ITIMER_PROF    2

struct itimerval
{
	struct timeval it_interval;
	struct timeval it_value;
};

int getitimer (int, struct itimerval *);
int setitimer (int, const struct itimerval *, struct itimerval *);
int utimes (const char *, const struct timeval [2]);

#ifdef _GNU_SOURCE
struct timezone {
	int tz_minuteswest;
	int tz_dsttime;
};
#endif

#ifdef __cplusplus
}
#endif
#endif
