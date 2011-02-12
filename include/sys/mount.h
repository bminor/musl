#ifndef _SYS_MOUNT_H
#define _SYS_MOUNT_H

#define MS_RDONLY      1
#define MS_NOSUID      2
#define MS_NODEV       4
#define MS_NOEXEC      8
#define MS_SYNCHRONOUS 16
#define MS_REMOUNT     32
#define MS_MANDLOCK    64
#define S_WRITE        128
#define S_APPEND       256
#define S_IMMUTABLE    512
#define MS_NOATIME     1024
#define MS_NODIRATIME  2048
#define MS_BIND        4096
#define MS_MOVE        8192
#define MS_SILENT      32768

#define MS_MGC_VAL 0xc0ed0000

#define MNT_FORCE 1

int mount(const char *, const char *, const char *, unsigned long, const void *);
int umount(const char *);
int umount2(const char *, int);

#endif
