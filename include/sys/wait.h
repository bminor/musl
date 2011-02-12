#ifndef	_SYS_WAIT_H
#define	_SYS_WAIT_H
#ifdef __cplusplus
extern "C" {
#endif

#define __NEED_pid_t
#define __NEED_id_t
#define __NEED_time_t
#define __NEED_struct_timeval
#define __NEED_siginfo_t
#include <bits/alltypes.h>

typedef int idtype_t;

pid_t wait (int *);
int waitid (idtype_t, id_t, siginfo_t *, int);
pid_t waitpid (pid_t, int *, int );
//pid_t wait3 (int *, int, struct rusage *);
//pid_t wait4 (pid_t, int *, int, struct rusage *);

#include <bits/wait.h>
#include <bits/wexitstatus.h>

#ifdef __cplusplus
}
#endif
#endif
