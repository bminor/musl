#ifndef _SYS_MSG_H
#define _SYS_MSG_H

#include <sys/ipc.h>

#define __NEED_pid_t
#define __NEED_key_t
#define __NEED_time_t
#define __NEED_size_t
#define __NEED_ssize_t

#include <bits/alltypes.h>

typedef unsigned long msgqnum_t;
typedef unsigned long msglen_t;

struct msqid_ds
{
	struct ipc_perm msg_perm;
	time_t msg_stime;
	time_t msg_rtime;
	time_t msg_ctime;
	msgqnum_t msg_qnum;
	msglen_t msg_qbytes;
	pid_t msg_lspid;
	pid_t msd_lrpid;
};

#define MSG_NOERROR 010000

int msgctl (int, int, struct msqid_ds *);
int msgget (key_t, int);
int msgrcv (int, void *, size_t, long, int);
int msgsnd (int, const void *, size_t, int);

#endif
