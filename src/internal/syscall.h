#ifndef _SYSCALL_H
#define _SYSCALL_H

/* This header is mostly useless leftover wrapper cruft */

#include <sys/syscall.h>

#define syscall0 syscall
#define syscall1 syscall
#define syscall2 syscall
#define syscall3 syscall
#define syscall4 syscall
#define syscall5 syscall
#define syscall6 syscall

#define socketcall __socketcall

/* the following are needed for iso c functions to use */
#define __syscall_open(filename, flags, mode) syscall(__NR_open, (filename), (flags)|0100000, (mode))
#define __syscall_read(fd, buf, len)          syscall(__NR_read, (fd), (buf), (len))
#define __syscall_write(fd, buf, len)         syscall(__NR_write, (fd), (buf), (len))
#define __syscall_close(fd)                   syscall(__NR_close, (fd))
#define __syscall_fcntl(fd, cmd, arg)         syscall(__NR_fcntl, (fd), (cmd), (arg))
#define __syscall_dup2(old, new)              syscall(__NR_dup2, (old), (new))
#define __syscall_unlink(path)                syscall(__NR_unlink, (path))
#define __syscall_getpid()                    syscall(__NR_getpid)
#define __syscall_kill(pid,sig)               syscall(__NR_kill, (pid), (sig))
#define __syscall_sigaction(sig,new,old)      syscall(__NR_rt_sigaction, (sig), (new), (old), 8)
#define __syscall_ioctl(fd,ioc,arg)           syscall(__NR_ioctl, (fd), (ioc), (arg))
#define __syscall_exit(code)                  syscall(__NR_exit, code)

#endif
