#define __NR_read (0x40000000 + 0)
#define __NR_write (0x40000000 + 1)
#define __NR_open (0x40000000 + 2)
#define __NR_close (0x40000000 + 3)
#define __NR_stat (0x40000000 + 4)
#define __NR_fstat (0x40000000 + 5)
#define __NR_lstat (0x40000000 + 6)
#define __NR_poll (0x40000000 + 7)
#define __NR_lseek (0x40000000 + 8)
#define __NR_mmap (0x40000000 + 9)
#define __NR_mprotect (0x40000000 + 10)
#define __NR_munmap (0x40000000 + 11)
#define __NR_brk (0x40000000 + 12)
#define __NR_rt_sigprocmask (0x40000000 + 14)
#define __NR_pread64 (0x40000000 + 17)
#define __NR_pwrite64 (0x40000000 + 18)
#define __NR_access (0x40000000 + 21)
#define __NR_pipe (0x40000000 + 22)
#define __NR_select (0x40000000 + 23)
#define __NR_sched_yield (0x40000000 + 24)
#define __NR_mremap (0x40000000 + 25)
#define __NR_msync (0x40000000 + 26)
#define __NR_mincore (0x40000000 + 27)
#define __NR_madvise (0x40000000 + 28)
#define __NR_shmget (0x40000000 + 29)
#define __NR_shmat (0x40000000 + 30)
#define __NR_shmctl (0x40000000 + 31)
#define __NR_dup (0x40000000 + 32)
#define __NR_dup2 (0x40000000 + 33)
#define __NR_pause (0x40000000 + 34)
#define __NR_nanosleep (0x40000000 + 35)
#define __NR_getitimer (0x40000000 + 36)
#define __NR_alarm (0x40000000 + 37)
#define __NR_setitimer (0x40000000 + 38)
#define __NR_getpid (0x40000000 + 39)
#define __NR_sendfile (0x40000000 + 40)
#define __NR_socket (0x40000000 + 41)
#define __NR_connect (0x40000000 + 42)
#define __NR_accept (0x40000000 + 43)
#define __NR_sendto (0x40000000 + 44)
#define __NR_shutdown (0x40000000 + 48)
#define __NR_bind (0x40000000 + 49)
#define __NR_listen (0x40000000 + 50)
#define __NR_getsockname (0x40000000 + 51)
#define __NR_getpeername (0x40000000 + 52)
#define __NR_socketpair (0x40000000 + 53)
#define __NR_clone (0x40000000 + 56)
#define __NR_fork (0x40000000 + 57)
#define __NR_vfork (0x40000000 + 58)
#define __NR_exit (0x40000000 + 60)
#define __NR_wait4 (0x40000000 + 61)
#define __NR_kill (0x40000000 + 62)
#define __NR_uname (0x40000000 + 63)
#define __NR_semget (0x40000000 + 64)
#define __NR_semop (0x40000000 + 65)
#define __NR_semctl (0x40000000 + 66)
#define __NR_shmdt (0x40000000 + 67)
#define __NR_msgget (0x40000000 + 68)
#define __NR_msgsnd (0x40000000 + 69)
#define __NR_msgrcv (0x40000000 + 70)
#define __NR_msgctl (0x40000000 + 71)
#define __NR_fcntl (0x40000000 + 72)
#define __NR_flock (0x40000000 + 73)
#define __NR_fsync (0x40000000 + 74)
#define __NR_fdatasync (0x40000000 + 75)
#define __NR_truncate (0x40000000 + 76)
#define __NR_ftruncate (0x40000000 + 77)
#define __NR_getdents (0x40000000 + 78)
#define __NR_getcwd (0x40000000 + 79)
#define __NR_chdir (0x40000000 + 80)
#define __NR_fchdir (0x40000000 + 81)
#define __NR_rename (0x40000000 + 82)
#define __NR_mkdir (0x40000000 + 83)
#define __NR_rmdir (0x40000000 + 84)
#define __NR_creat (0x40000000 + 85)
#define __NR_link (0x40000000 + 86)
#define __NR_unlink (0x40000000 + 87)
#define __NR_symlink (0x40000000 + 88)
#define __NR_readlink (0x40000000 + 89)
#define __NR_chmod (0x40000000 + 90)
#define __NR_fchmod (0x40000000 + 91)
#define __NR_chown (0x40000000 + 92)
#define __NR_fchown (0x40000000 + 93)
#define __NR_lchown (0x40000000 + 94)
#define __NR_umask (0x40000000 + 95)
#define __NR_gettimeofday (0x40000000 + 96)
#define __NR_getrlimit (0x40000000 + 97)
#define __NR_getrusage (0x40000000 + 98)
#define __NR_sysinfo (0x40000000 + 99)
#define __NR_times (0x40000000 + 100)
#define __NR_getuid (0x40000000 + 102)
#define __NR_syslog (0x40000000 + 103)
#define __NR_getgid (0x40000000 + 104)
#define __NR_setuid (0x40000000 + 105)
#define __NR_setgid (0x40000000 + 106)
#define __NR_geteuid (0x40000000 + 107)
#define __NR_getegid (0x40000000 + 108)
#define __NR_setpgid (0x40000000 + 109)
#define __NR_getppid (0x40000000 + 110)
#define __NR_getpgrp (0x40000000 + 111)
#define __NR_setsid (0x40000000 + 112)
#define __NR_setreuid (0x40000000 + 113)
#define __NR_setregid (0x40000000 + 114)
#define __NR_getgroups (0x40000000 + 115)
#define __NR_setgroups (0x40000000 + 116)
#define __NR_setresuid (0x40000000 + 117)
#define __NR_getresuid (0x40000000 + 118)
#define __NR_setresgid (0x40000000 + 119)
#define __NR_getresgid (0x40000000 + 120)
#define __NR_getpgid (0x40000000 + 121)
#define __NR_setfsuid (0x40000000 + 122)
#define __NR_setfsgid (0x40000000 + 123)
#define __NR_getsid (0x40000000 + 124)
#define __NR_capget (0x40000000 + 125)
#define __NR_capset (0x40000000 + 126)
#define __NR_rt_sigsuspend (0x40000000 + 130)
#define __NR_utime (0x40000000 + 132)
#define __NR_mknod (0x40000000 + 133)
#define __NR_personality (0x40000000 + 135)
#define __NR_ustat (0x40000000 + 136)
#define __NR_statfs (0x40000000 + 137)
#define __NR_fstatfs (0x40000000 + 138)
#define __NR_sysfs (0x40000000 + 139)
#define __NR_getpriority (0x40000000 + 140)
#define __NR_setpriority (0x40000000 + 141)
#define __NR_sched_setparam (0x40000000 + 142)
#define __NR_sched_getparam (0x40000000 + 143)
#define __NR_sched_setscheduler (0x40000000 + 144)
#define __NR_sched_getscheduler (0x40000000 + 145)
#define __NR_sched_get_priority_max (0x40000000 + 146)
#define __NR_sched_get_priority_min (0x40000000 + 147)
#define __NR_sched_rr_get_interval (0x40000000 + 148)
#define __NR_mlock (0x40000000 + 149)
#define __NR_munlock (0x40000000 + 150)
#define __NR_mlockall (0x40000000 + 151)
#define __NR_munlockall (0x40000000 + 152)
#define __NR_vhangup (0x40000000 + 153)
#define __NR_modify_ldt (0x40000000 + 154)
#define __NR_pivot_root (0x40000000 + 155)
#define __NR_prctl (0x40000000 + 157)
#define __NR_arch_prctl (0x40000000 + 158)
#define __NR_adjtimex (0x40000000 + 159)
#define __NR_setrlimit (0x40000000 + 160)
#define __NR_chroot (0x40000000 + 161)
#define __NR_sync (0x40000000 + 162)
#define __NR_acct (0x40000000 + 163)
#define __NR_settimeofday (0x40000000 + 164)
#define __NR_mount (0x40000000 + 165)
#define __NR_umount2 (0x40000000 + 166)
#define __NR_swapon (0x40000000 + 167)
#define __NR_swapoff (0x40000000 + 168)
#define __NR_reboot (0x40000000 + 169)
#define __NR_sethostname (0x40000000 + 170)
#define __NR_setdomainname (0x40000000 + 171)
#define __NR_iopl (0x40000000 + 172)
#define __NR_ioperm (0x40000000 + 173)
#define __NR_init_module (0x40000000 + 175)
#define __NR_delete_module (0x40000000 + 176)
#define __NR_quotactl (0x40000000 + 179)
#define __NR_getpmsg (0x40000000 + 181)
#define __NR_putpmsg (0x40000000 + 182)
#define __NR_afs_syscall (0x40000000 + 183)
#define __NR_tuxcall (0x40000000 + 184)
#define __NR_security (0x40000000 + 185)
#define __NR_gettid (0x40000000 + 186)
#define __NR_readahead (0x40000000 + 187)
#define __NR_setxattr (0x40000000 + 188)
#define __NR_lsetxattr (0x40000000 + 189)
#define __NR_fsetxattr (0x40000000 + 190)
#define __NR_getxattr (0x40000000 + 191)
#define __NR_lgetxattr (0x40000000 + 192)
#define __NR_fgetxattr (0x40000000 + 193)
#define __NR_listxattr (0x40000000 + 194)
#define __NR_llistxattr (0x40000000 + 195)
#define __NR_flistxattr (0x40000000 + 196)
#define __NR_removexattr (0x40000000 + 197)
#define __NR_lremovexattr (0x40000000 + 198)
#define __NR_fremovexattr (0x40000000 + 199)
#define __NR_tkill (0x40000000 + 200)
#define __NR_time (0x40000000 + 201)
#define __NR_futex (0x40000000 + 202)
#define __NR_sched_setaffinity (0x40000000 + 203)
#define __NR_sched_getaffinity (0x40000000 + 204)
#define __NR_io_destroy (0x40000000 + 207)
#define __NR_io_getevents (0x40000000 + 208)
#define __NR_io_cancel (0x40000000 + 210)
#define __NR_lookup_dcookie (0x40000000 + 212)
#define __NR_epoll_create (0x40000000 + 213)
#define __NR_remap_file_pages (0x40000000 + 216)
#define __NR_getdents64 (0x40000000 + 217)
#define __NR_set_tid_address (0x40000000 + 218)
#define __NR_restart_syscall (0x40000000 + 219)
#define __NR_semtimedop (0x40000000 + 220)
#define __NR_fadvise64 (0x40000000 + 221)
#define __NR_timer_settime (0x40000000 + 223)
#define __NR_timer_gettime (0x40000000 + 224)
#define __NR_timer_getoverrun (0x40000000 + 225)
#define __NR_timer_delete (0x40000000 + 226)
#define __NR_clock_settime (0x40000000 + 227)
#define __NR_clock_gettime (0x40000000 + 228)
#define __NR_clock_getres (0x40000000 + 229)
#define __NR_clock_nanosleep (0x40000000 + 230)
#define __NR_exit_group (0x40000000 + 231)
#define __NR_epoll_wait (0x40000000 + 232)
#define __NR_epoll_ctl (0x40000000 + 233)
#define __NR_tgkill (0x40000000 + 234)
#define __NR_utimes (0x40000000 + 235)
#define __NR_mbind (0x40000000 + 237)
#define __NR_set_mempolicy (0x40000000 + 238)
#define __NR_get_mempolicy (0x40000000 + 239)
#define __NR_mq_open (0x40000000 + 240)
#define __NR_mq_unlink (0x40000000 + 241)
#define __NR_mq_timedsend (0x40000000 + 242)
#define __NR_mq_timedreceive (0x40000000 + 243)
#define __NR_mq_getsetattr (0x40000000 + 245)
#define __NR_add_key (0x40000000 + 248)
#define __NR_request_key (0x40000000 + 249)
#define __NR_keyctl (0x40000000 + 250)
#define __NR_ioprio_set (0x40000000 + 251)
#define __NR_ioprio_get (0x40000000 + 252)
#define __NR_inotify_init (0x40000000 + 253)
#define __NR_inotify_add_watch (0x40000000 + 254)
#define __NR_inotify_rm_watch (0x40000000 + 255)
#define __NR_migrate_pages (0x40000000 + 256)
#define __NR_openat (0x40000000 + 257)
#define __NR_mkdirat (0x40000000 + 258)
#define __NR_mknodat (0x40000000 + 259)
#define __NR_fchownat (0x40000000 + 260)
#define __NR_futimesat (0x40000000 + 261)
#define __NR_newfstatat (0x40000000 + 262)
#define __NR_unlinkat (0x40000000 + 263)
#define __NR_renameat (0x40000000 + 264)
#define __NR_linkat (0x40000000 + 265)
#define __NR_symlinkat (0x40000000 + 266)
#define __NR_readlinkat (0x40000000 + 267)
#define __NR_fchmodat (0x40000000 + 268)
#define __NR_faccessat (0x40000000 + 269)
#define __NR_pselect6 (0x40000000 + 270)
#define __NR_ppoll (0x40000000 + 271)
#define __NR_unshare (0x40000000 + 272)
#define __NR_splice (0x40000000 + 275)
#define __NR_tee (0x40000000 + 276)
#define __NR_sync_file_range (0x40000000 + 277)
#define __NR_utimensat (0x40000000 + 280)
#define __NR_epoll_pwait (0x40000000 + 281)
#define __NR_signalfd (0x40000000 + 282)
#define __NR_timerfd_create (0x40000000 + 283)
#define __NR_eventfd (0x40000000 + 284)
#define __NR_fallocate (0x40000000 + 285)
#define __NR_timerfd_settime (0x40000000 + 286)
#define __NR_timerfd_gettime (0x40000000 + 287)
#define __NR_accept4 (0x40000000 + 288)
#define __NR_signalfd4 (0x40000000 + 289)
#define __NR_eventfd2 (0x40000000 + 290)
#define __NR_epoll_create1 (0x40000000 + 291)
#define __NR_dup3 (0x40000000 + 292)
#define __NR_pipe2 (0x40000000 + 293)
#define __NR_inotify_init1 (0x40000000 + 294)
#define __NR_perf_event_open (0x40000000 + 298)
#define __NR_fanotify_init (0x40000000 + 300)
#define __NR_fanotify_mark (0x40000000 + 301)
#define __NR_prlimit64 (0x40000000 + 302)
#define __NR_name_to_handle_at (0x40000000 + 303)
#define __NR_open_by_handle_at (0x40000000 + 304)
#define __NR_clock_adjtime (0x40000000 + 305)
#define __NR_syncfs (0x40000000 + 306)
#define __NR_setns (0x40000000 + 308)
#define __NR_getcpu (0x40000000 + 309)
#define __NR_kcmp (0x40000000 + 312)
#define __NR_finit_module (0x40000000 + 313)
#define __NR_sched_setattr (0x40000000 + 314)
#define __NR_sched_getattr (0x40000000 + 315)
#define __NR_renameat2 (0x40000000 + 316)
#define __NR_seccomp (0x40000000 + 317)
#define __NR_getrandom (0x40000000 + 318)
#define __NR_memfd_create (0x40000000 + 319)
#define __NR_kexec_file_load (0x40000000 + 320)
#define __NR_bpf (0x40000000 + 321)
#define __NR_userfaultfd (0x40000000 + 323)
#define __NR_membarrier (0x40000000 + 324)
#define __NR_mlock2 (0x40000000 + 325)
#define __NR_copy_file_range (0x40000000 + 326)

#define __NR_rt_sigaction (0x40000000 + 512)
#define __NR_rt_sigreturn (0x40000000 + 513)
#define __NR_ioctl (0x40000000 + 514)
#define __NR_readv (0x40000000 + 515)
#define __NR_writev (0x40000000 + 516)
#define __NR_recvfrom (0x40000000 + 517)
#define __NR_sendmsg (0x40000000 + 518)
#define __NR_recvmsg (0x40000000 + 519)
#define __NR_execve (0x40000000 + 520)
#define __NR_ptrace (0x40000000 + 521)
#define __NR_rt_sigpending (0x40000000 + 522)
#define __NR_rt_sigtimedwait (0x40000000 + 523)
#define __NR_rt_sigqueueinfo (0x40000000 + 524)
#define __NR_sigaltstack (0x40000000 + 525)
#define __NR_timer_create (0x40000000 + 526)
#define __NR_mq_notify (0x40000000 + 527)
#define __NR_kexec_load (0x40000000 + 528)
#define __NR_waitid (0x40000000 + 529)
#define __NR_set_robust_list (0x40000000 + 530)
#define __NR_get_robust_list (0x40000000 + 531)
#define __NR_vmsplice (0x40000000 + 532)
#define __NR_move_pages (0x40000000 + 533)
#define __NR_preadv (0x40000000 + 534)
#define __NR_pwritev (0x40000000 + 535)
#define __NR_rt_tgsigqueueinfo (0x40000000 + 536)
#define __NR_recvmmsg (0x40000000 + 537)
#define __NR_sendmmsg (0x40000000 + 538)
#define __NR_process_vm_readv (0x40000000 + 539)
#define __NR_process_vm_writev (0x40000000 + 540)
#define __NR_setsockopt (0x40000000 + 541)
#define __NR_getsockopt (0x40000000 + 542)
#define __NR_io_setup (0x40000000 + 543)
#define __NR_io_submit (0x40000000 + 544)
#define __NR_execveat (0x40000000 + 545)



/* Repeat with SYS_ prefix */



#define SYS_read __NR_read
#define SYS_write __NR_write
#define SYS_open __NR_open
#define SYS_close __NR_close
#define SYS_stat __NR_stat
#define SYS_fstat __NR_fstat
#define SYS_lstat __NR_lstat
#define SYS_poll __NR_poll
#define SYS_lseek __NR_lseek
#define SYS_mmap __NR_mmap
#define SYS_mprotect __NR_mprotect
#define SYS_munmap __NR_munmap
#define SYS_brk __NR_brk
#define SYS_rt_sigprocmask __NR_rt_sigprocmask
#define SYS_pread64 __NR_pread64
#define SYS_pwrite64 __NR_pwrite64
#define SYS_access __NR_access
#define SYS_pipe __NR_pipe
#define SYS_select __NR_select
#define SYS_sched_yield __NR_sched_yield
#define SYS_mremap __NR_mremap
#define SYS_msync __NR_msync
#define SYS_mincore __NR_mincore
#define SYS_madvise __NR_madvise
#define SYS_shmget __NR_shmget
#define SYS_shmat __NR_shmat
#define SYS_shmctl __NR_shmctl
#define SYS_dup __NR_dup
#define SYS_dup2 __NR_dup2
#define SYS_pause __NR_pause
#define SYS_nanosleep __NR_nanosleep
#define SYS_getitimer __NR_getitimer
#define SYS_alarm __NR_alarm
#define SYS_setitimer __NR_setitimer
#define SYS_getpid __NR_getpid
#define SYS_sendfile __NR_sendfile
#define SYS_socket __NR_socket
#define SYS_connect __NR_connect
#define SYS_accept __NR_accept
#define SYS_sendto __NR_sendto
#define SYS_shutdown __NR_shutdown
#define SYS_bind __NR_bind
#define SYS_listen __NR_listen
#define SYS_getsockname __NR_getsockname
#define SYS_getpeername __NR_getpeername
#define SYS_socketpair __NR_socketpair
#define SYS_clone __NR_clone
#define SYS_fork __NR_fork
#define SYS_vfork __NR_vfork
#define SYS_exit __NR_exit
#define SYS_wait4 __NR_wait4
#define SYS_kill __NR_kill
#define SYS_uname __NR_uname
#define SYS_semget __NR_semget
#define SYS_semop __NR_semop
#define SYS_semctl __NR_semctl
#define SYS_shmdt __NR_shmdt
#define SYS_msgget __NR_msgget
#define SYS_msgsnd __NR_msgsnd
#define SYS_msgrcv __NR_msgrcv
#define SYS_msgctl __NR_msgctl
#define SYS_fcntl __NR_fcntl
#define SYS_flock __NR_flock
#define SYS_fsync __NR_fsync
#define SYS_fdatasync __NR_fdatasync
#define SYS_truncate __NR_truncate
#define SYS_ftruncate __NR_ftruncate
#define SYS_getdents __NR_getdents
#define SYS_getcwd __NR_getcwd
#define SYS_chdir __NR_chdir
#define SYS_fchdir __NR_fchdir
#define SYS_rename __NR_rename
#define SYS_mkdir __NR_mkdir
#define SYS_rmdir __NR_rmdir
#define SYS_creat __NR_creat
#define SYS_link __NR_link
#define SYS_unlink __NR_unlink
#define SYS_symlink __NR_symlink
#define SYS_readlink __NR_readlink
#define SYS_chmod __NR_chmod
#define SYS_fchmod __NR_fchmod
#define SYS_chown __NR_chown
#define SYS_fchown __NR_fchown
#define SYS_lchown __NR_lchown
#define SYS_umask __NR_umask
#define SYS_gettimeofday __NR_gettimeofday
#define SYS_getrlimit __NR_getrlimit
#define SYS_getrusage __NR_getrusage
#define SYS_sysinfo __NR_sysinfo
#define SYS_times __NR_times
#define SYS_getuid __NR_getuid
#define SYS_syslog __NR_syslog
#define SYS_getgid __NR_getgid
#define SYS_setuid __NR_setuid
#define SYS_setgid __NR_setgid
#define SYS_geteuid __NR_geteuid
#define SYS_getegid __NR_getegid
#define SYS_setpgid __NR_setpgid
#define SYS_getppid __NR_getppid
#define SYS_getpgrp __NR_getpgrp
#define SYS_setsid __NR_setsid
#define SYS_setreuid __NR_setreuid
#define SYS_setregid __NR_setregid
#define SYS_getgroups __NR_getgroups
#define SYS_setgroups __NR_setgroups
#define SYS_setresuid __NR_setresuid
#define SYS_getresuid __NR_getresuid
#define SYS_setresgid __NR_setresgid
#define SYS_getresgid __NR_getresgid
#define SYS_getpgid __NR_getpgid
#define SYS_setfsuid __NR_setfsuid
#define SYS_setfsgid __NR_setfsgid
#define SYS_getsid __NR_getsid
#define SYS_capget __NR_capget
#define SYS_capset __NR_capset
#define SYS_rt_sigsuspend __NR_rt_sigsuspend
#define SYS_utime __NR_utime
#define SYS_mknod __NR_mknod
#define SYS_personality __NR_personality
#define SYS_ustat __NR_ustat
#define SYS_statfs __NR_statfs
#define SYS_fstatfs __NR_fstatfs
#define SYS_sysfs __NR_sysfs
#define SYS_getpriority __NR_getpriority
#define SYS_setpriority __NR_setpriority
#define SYS_sched_setparam __NR_sched_setparam
#define SYS_sched_getparam __NR_sched_getparam
#define SYS_sched_setscheduler __NR_sched_setscheduler
#define SYS_sched_getscheduler __NR_sched_getscheduler
#define SYS_sched_get_priority_max __NR_sched_get_priority_max
#define SYS_sched_get_priority_min __NR_sched_get_priority_min
#define SYS_sched_rr_get_interval __NR_sched_rr_get_interval
#define SYS_mlock __NR_mlock
#define SYS_munlock __NR_munlock
#define SYS_mlockall __NR_mlockall
#define SYS_munlockall __NR_munlockall
#define SYS_vhangup __NR_vhangup
#define SYS_modify_ldt __NR_modify_ldt
#define SYS_pivot_root __NR_pivot_root
#define SYS_prctl __NR_prctl
#define SYS_arch_prctl __NR_arch_prctl
#define SYS_adjtimex __NR_adjtimex
#define SYS_setrlimit __NR_setrlimit
#define SYS_chroot __NR_chroot
#define SYS_sync __NR_sync
#define SYS_acct __NR_acct
#define SYS_settimeofday __NR_settimeofday
#define SYS_mount __NR_mount
#define SYS_umount2 __NR_umount2
#define SYS_swapon __NR_swapon
#define SYS_swapoff __NR_swapoff
#define SYS_reboot __NR_reboot
#define SYS_sethostname __NR_sethostname
#define SYS_setdomainname __NR_setdomainname
#define SYS_iopl __NR_iopl
#define SYS_ioperm __NR_ioperm
#define SYS_init_module __NR_init_module
#define SYS_delete_module __NR_delete_module
#define SYS_quotactl __NR_quotactl
#define SYS_getpmsg __NR_getpmsg
#define SYS_putpmsg __NR_putpmsg
#define SYS_afs_syscall __NR_afs_syscall
#define SYS_tuxcall __NR_tuxcall
#define SYS_security __NR_security
#define SYS_gettid __NR_gettid
#define SYS_readahead __NR_readahead
#define SYS_setxattr __NR_setxattr
#define SYS_lsetxattr __NR_lsetxattr
#define SYS_fsetxattr __NR_fsetxattr
#define SYS_getxattr __NR_getxattr
#define SYS_lgetxattr __NR_lgetxattr
#define SYS_fgetxattr __NR_fgetxattr
#define SYS_listxattr __NR_listxattr
#define SYS_llistxattr __NR_llistxattr
#define SYS_flistxattr __NR_flistxattr
#define SYS_removexattr __NR_removexattr
#define SYS_lremovexattr __NR_lremovexattr
#define SYS_fremovexattr __NR_fremovexattr
#define SYS_tkill __NR_tkill
#define SYS_time __NR_time
#define SYS_futex __NR_futex
#define SYS_sched_setaffinity __NR_sched_setaffinity
#define SYS_sched_getaffinity __NR_sched_getaffinity
#define SYS_io_destroy __NR_io_destroy
#define SYS_io_getevents __NR_io_getevents
#define SYS_io_cancel __NR_io_cancel
#define SYS_lookup_dcookie __NR_lookup_dcookie
#define SYS_epoll_create __NR_epoll_create
#define SYS_remap_file_pages __NR_remap_file_pages
#define SYS_getdents64 __NR_getdents64
#define SYS_set_tid_address __NR_set_tid_address
#define SYS_restart_syscall __NR_restart_syscall
#define SYS_semtimedop __NR_semtimedop
#define SYS_fadvise64 __NR_fadvise64
#define SYS_timer_settime __NR_timer_settime
#define SYS_timer_gettime __NR_timer_gettime
#define SYS_timer_getoverrun __NR_timer_getoverrun
#define SYS_timer_delete __NR_timer_delete
#define SYS_clock_settime __NR_clock_settime
#define SYS_clock_gettime __NR_clock_gettime
#define SYS_clock_getres __NR_clock_getres
#define SYS_clock_nanosleep __NR_clock_nanosleep
#define SYS_exit_group __NR_exit_group
#define SYS_epoll_wait __NR_epoll_wait
#define SYS_epoll_ctl __NR_epoll_ctl
#define SYS_tgkill __NR_tgkill
#define SYS_utimes __NR_utimes
#define SYS_mbind __NR_mbind
#define SYS_set_mempolicy __NR_set_mempolicy
#define SYS_get_mempolicy __NR_get_mempolicy
#define SYS_mq_open __NR_mq_open
#define SYS_mq_unlink __NR_mq_unlink
#define SYS_mq_timedsend __NR_mq_timedsend
#define SYS_mq_timedreceive __NR_mq_timedreceive
#define SYS_mq_getsetattr __NR_mq_getsetattr
#define SYS_add_key __NR_add_key
#define SYS_request_key __NR_request_key
#define SYS_keyctl __NR_keyctl
#define SYS_ioprio_set __NR_ioprio_set
#define SYS_ioprio_get __NR_ioprio_get
#define SYS_inotify_init __NR_inotify_init
#define SYS_inotify_add_watch __NR_inotify_add_watch
#define SYS_inotify_rm_watch __NR_inotify_rm_watch
#define SYS_migrate_pages __NR_migrate_pages
#define SYS_openat __NR_openat
#define SYS_mkdirat __NR_mkdirat
#define SYS_mknodat __NR_mknodat
#define SYS_fchownat __NR_fchownat
#define SYS_futimesat __NR_futimesat
#define SYS_newfstatat __NR_newfstatat
#define SYS_unlinkat __NR_unlinkat
#define SYS_renameat __NR_renameat
#define SYS_linkat __NR_linkat
#define SYS_symlinkat __NR_symlinkat
#define SYS_readlinkat __NR_readlinkat
#define SYS_fchmodat __NR_fchmodat
#define SYS_faccessat __NR_faccessat
#define SYS_pselect6 __NR_pselect6
#define SYS_ppoll __NR_ppoll
#define SYS_unshare __NR_unshare
#define SYS_splice __NR_splice
#define SYS_tee __NR_tee
#define SYS_sync_file_range __NR_sync_file_range
#define SYS_utimensat __NR_utimensat
#define SYS_epoll_pwait __NR_epoll_pwait
#define SYS_signalfd __NR_signalfd
#define SYS_timerfd_create __NR_timerfd_create
#define SYS_eventfd __NR_eventfd
#define SYS_fallocate __NR_fallocate
#define SYS_timerfd_settime __NR_timerfd_settime
#define SYS_timerfd_gettime __NR_timerfd_gettime
#define SYS_accept4 __NR_accept4
#define SYS_signalfd4 __NR_signalfd4
#define SYS_eventfd2 __NR_eventfd2
#define SYS_epoll_create1 __NR_epoll_create1
#define SYS_dup3 __NR_dup3
#define SYS_pipe2 __NR_pipe2
#define SYS_inotify_init1 __NR_inotify_init1
#define SYS_perf_event_open __NR_perf_event_open
#define SYS_fanotify_init __NR_fanotify_init
#define SYS_fanotify_mark __NR_fanotify_mark
#define SYS_prlimit64 __NR_prlimit64
#define SYS_name_to_handle_at __NR_name_to_handle_at
#define SYS_open_by_handle_at __NR_open_by_handle_at
#define SYS_clock_adjtime __NR_clock_adjtime
#define SYS_syncfs __NR_syncfs
#define SYS_setns __NR_setns
#define SYS_getcpu __NR_getcpu
#define SYS_kcmp __NR_kcmp
#define SYS_finit_module __NR_finit_module
#define SYS_sched_setattr __NR_sched_setattr
#define SYS_sched_getattr __NR_sched_getattr
#define SYS_renameat2 __NR_renameat2
#define SYS_seccomp __NR_seccomp
#define SYS_getrandom __NR_getrandom
#define SYS_memfd_create __NR_memfd_create
#define SYS_kexec_file_load __NR_kexec_file_load
#define SYS_bpf __NR_bpf
#define SYS_userfaultfd __NR_userfaultfd
#define SYS_membarrier __NR_membarrier
#define SYS_mlock2 __NR_mlock2
#define SYS_copy_file_range __NR_copy_file_range


#define SYS_rt_sigaction __NR_rt_sigaction
#define SYS_rt_sigreturn __NR_rt_sigreturn
#define SYS_ioctl __NR_ioctl
#define SYS_readv __NR_readv
#define SYS_writev __NR_writev
#define SYS_recvfrom __NR_recvfrom
#define SYS_sendmsg __NR_sendmsg
#define SYS_recvmsg __NR_recvmsg
#define SYS_execve __NR_execve
#define SYS_ptrace __NR_ptrace
#define SYS_rt_sigpending __NR_rt_sigpending
#define SYS_rt_sigtimedwait __NR_rt_sigtimedwait
#define SYS_rt_sigqueueinfo __NR_rt_sigqueueinfo
#define SYS_sigaltstack __NR_sigaltstack
#define SYS_timer_create __NR_timer_create
#define SYS_mq_notify __NR_mq_notify
#define SYS_kexec_load __NR_kexec_load
#define SYS_waitid __NR_waitid
#define SYS_set_robust_list __NR_set_robust_list
#define SYS_get_robust_list __NR_get_robust_list
#define SYS_vmsplice __NR_vmsplice
#define SYS_move_pages __NR_move_pages
#define SYS_preadv __NR_preadv
#define SYS_pwritev __NR_pwritev
#define SYS_rt_tgsigqueueinfo __NR_rt_tgsigqueueinfo
#define SYS_recvmmsg __NR_recvmmsg
#define SYS_sendmmsg __NR_sendmmsg
#define SYS_process_vm_readv __NR_process_vm_readv
#define SYS_process_vm_writev __NR_process_vm_writev
#define SYS_setsockopt __NR_setsockopt
#define SYS_getsockopt __NR_getsockopt
#define SYS_io_setup __NR_io_setup
#define SYS_io_submit __NR_io_submit
#define SYS_execveat __NR_execveat
