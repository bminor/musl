#include <bits/asm.h>
        .global __syscall
        .type   __syscall,@function
__syscall:
        mflr    r0
        stw     r0, -4(r1)              // Save the return address.
        mr      r0, r3                  // Save the system call number
        mr      r3, r4                  // Shift the arguments: arg1
        mr      r4, r5                  // arg2
        mr      r5, r6                  // arg3
        mr      r6, r7                  // arg4
        mr      r7, r8                  // arg5
        mr      r8, r9                  // arg6
        sc
        mfcr    r0                      // Check for an error
        rlwinm  r4, r0, r0, 3, 3        // by checking for bit 28.
        cmplwi  r0, r4, 0               // It is an error if non-zero.
        beq     r0, 1f                  // Jump if not an error.
        neg     r3, r3                  // Negate the error number.
1:      lwz     r0, -4(r1)              // Restore the return address.
        mtlr    r0
	blr
        .end    __syscall
        .size   __syscall, .-__syscall
