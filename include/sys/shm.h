#ifndef _SYS_SHM_H
#define _SYS_SHM_H

#define __NEED_time_t
#define __NEED_size_t
#define __NEED_pid_t

#include <bits/alltypes.h>

#include <sys/ipc.h>
#include <bits/shm.h>

void *shmat(int, const void *, int);
int shmctl(int, int, struct shmid_ds *);
int shmdt(const void *);
int shmget(key_t, size_t, int);

#endif
