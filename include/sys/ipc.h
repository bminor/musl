#ifndef _SYS_IPC_H
#define _SYS_IPC_H
#ifdef __cplusplus
extern "C" {
#endif

#define __NEED_uid_t
#define __NEED_gid_t
#define __NEED_mode_t
#define __NEED_key_t

#include <bits/alltypes.h>

#include <bits/ipc.h>

key_t ftok (const char *, int);

#ifdef __cplusplus
}
#endif
#endif


