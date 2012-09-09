#define _GNU_SOURCE
#include <unistd.h>
#include <errno.h>
#include "syscall.h"

int dup3(int old, int new, int flags) {
        int r;
        while ((r=__syscall(SYS_dup3, old, new, flags))==-EBUSY);
        return __syscall_ret(r);
}
