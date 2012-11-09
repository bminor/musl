#include <bits/asm.h>
        .global __setjmp
        .global _setjmp
        .global setjmp
        .type   __setjmp,@function
        .type   _setjmp,@function
        .type   setjmp,@function
__setjmp:
_setjmp:
setjmp:
        mflr    r11
        mfcr    r12
        mr      r10, r1
        mr      r9, r2
        stmw    r8, 0(r3)               // save r8-r31
        li      r3,0
        blr

