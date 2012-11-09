#include <bits/asm.h>
        .text
        .global dlsym
        .type   dlsym,@function
dlsym:
        mflr    r5                      // The return address is arg3.
        b       __dlsym
        .end    dlsym
        .size   dlsym, .-dlsym
