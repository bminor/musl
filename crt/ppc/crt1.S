#include <bits/asm.h>
        .weak  _init
        .weak  _fini
        .global _start
        .type   _start, %function
_start:
        mr      r9, r1                  // Save the original stack pointer.
        clrrwi  r1, r1, 4               // Align the stack to 16 bytes.
        lis     r13, _SDA_BASE_@ha      // r13 points to the small data area.
        addi    r13, r13, _SDA_BASE_@l  //
        li      r0, 0                   // Zero the frame pointer.
        stwu    r1, -16(r1)             // The initial stack frame.
        mtlr    r0                      // Clear the link register.
        stw     r0, 0(r1)               // And save it.
        lis     r3, main@ha             // Get main() ...
        addi    r3, r3, main@l
        lwz     r4, 0(r9)               // and argc...
        addi    r5, r9, 4               // and argv ...
        lis     r6, _init@ha            // and _init() ...
        addi    r6, r6, _init@l
        lis     r7, _fini@ha            // and _fini() ...
        addi    r7, r7, _fini@l
        li      r8, 0                   // ldso_fini == NULL
        bl       __libc_start_main      // Let's go!
        b       .                       // Never gets here.
        .end    _start
        .size   _start, .-_start
