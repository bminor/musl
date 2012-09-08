#ifndef _SYS_IPC_H
#define _SYS_IPC_H
#ifdef __cplusplus
extern "C" {
#endif

#include <features.h>

#define __NEED_uid_t
#define __NEED_gid_t
#define __NEED_mode_t
#define __NEED_key_t

#include <bits/alltypes.h>

#ifdef _GNU_SOURCE
#define __ipc_perm_key key
#define __ipc_perm_seq seq
#endif

#include <bits/ipc.h>

#define IPC_CREAT  01000
#define IPC_EXCL   02000
#define IPC_NOWAIT 04000

#define IPC_RMID 0
#define IPC_SET  1
#define IPC_STAT 2
#define IPC_INFO 3

#define IPC_PRIVATE ((key_t) 0)

key_t ftok (const char *, int);

#ifdef __cplusplus
}
#endif
#endif
