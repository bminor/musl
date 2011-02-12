#ifndef _UTMP_H
#define _UTMP_H

#include <utmpx.h>

#define ut_time ut_tv.tv_sec
#define ut_name ut_user
#define ut_exit __ut_exit
#define e_termination __e_termination
#define e_exit __e_exit
#define utmp utmpx
#define endutent endutxent
#define getutent getutxent
#define setutent setutxent
#define getutid getutxid
#define getutline getutxline
#define pututline pututxline
#define utmpname(x) (-1)

#define _PATH_UTMP "/dev/null"
#define _PATH_WTMP "/dev/null"

#endif
