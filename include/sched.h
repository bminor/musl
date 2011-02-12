#ifndef _SCHED_H
#define _SCHED_H
#ifdef __cplusplus
extern "C" {
#endif

#define __NEED_struct_timespec
#define __NEED_pid_t
#define __NEED_time_t

#include <bits/alltypes.h>

struct sched_param {
	int sched_priority;
	int sched_ss_low_priority;
	struct timespec sched_ss_repl_period;
	struct timespec sched_ss_init_budget;
	int sched_ss_max_repl;
};

int    sched_get_priority_max(int);
int    sched_get_priority_min(int);
int    sched_getparam(pid_t, struct sched_param *);
int    sched_getscheduler(pid_t);
int    sched_rr_get_interval(pid_t, struct timespec *);
int    sched_setparam(pid_t, const struct sched_param *);
int    sched_setscheduler(pid_t, int, const struct sched_param *);
int     sched_yield(void);

#define SCHED_OTHER 0
#define SCHED_FIFO 1
#define SCHED_RR 2

#ifdef __cplusplus
}
#endif
#endif
