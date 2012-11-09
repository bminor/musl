#include <bits/asm.h>
#include <bits/syscall.h>
        .text
        .global __unmapself
        .type   __unmapself,%function
__unmapself:
        li      r0, __NR_munmap
        sc
        li      r0, __NR_exit
        sc
        blr
