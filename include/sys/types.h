#ifndef	_SYS_TYPES_H
#define	_SYS_TYPES_H
#ifdef __cplusplus
extern "C" {
#endif

#define __NEED_loff_t
#define __NEED_ino_t
#define __NEED_dev_t
#define __NEED_uid_t
#define __NEED_gid_t
#define __NEED_mode_t
#define __NEED_nlink_t
#define __NEED_off_t
#define __NEED_pid_t
#define __NEED_size_t
#define __NEED_ssize_t
#define __NEED_time_t
#define __NEED_timer_t
#define __NEED_clockid_t

#define __NEED_int8_t
#define __NEED_int16_t
#define __NEED_int32_t
#define __NEED_int64_t

#define __NEED_u_int8_t
#define __NEED_u_int16_t
#define __NEED_u_int32_t
#define __NEED_u_int64_t

#define __NEED_register_t

#define __NEED_blkcnt_t
#define __NEED_fsblkcnt_t
#define __NEED_fsfilcnt_t

#define __NEED_id_t
#define __NEED_key_t
#define __NEED_clock_t
#define __NEED_useconds_t
#define __NEED_suseconds_t
#define __NEED_blksize_t

#include <bits/alltypes.h>

#ifdef _GNU_SOURCE
typedef unsigned long caddr_t;
#endif

#ifdef __cplusplus
}
#endif
#endif


