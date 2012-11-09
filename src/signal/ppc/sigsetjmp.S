#include <bits/asm.h>
        .global sigsetjmp
        .type sigsetjmp,@function
sigsetjmp:
        lwz     r4, 64*4-2*4(r3)        // Second last long.
        cmpi    r4, 0, r4, 0
        bne     r4, 1f
        addi    r5, r3, 64*4-1*4        // Address of last long.
        li      r4, 0
        li      r3, 2
        bl      sigprocmask
1:      b       setjmp
