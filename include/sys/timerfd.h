#ifndef _SYS_TIMERFD_H
#define _SYS_TIMERFD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <time.h>

int timerfd_create(int, int);
int timerfd_settime(int, int, const struct itimerspec *, struct itimerspec *);
int timerfd_gettime(int, struct itimerspec *);

#ifdef __cplusplus
}
#endif

#endif
