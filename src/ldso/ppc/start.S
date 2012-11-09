#include <bits/asm.h>
        .global _start
        .type   _start,@function
_start:
        mr      r9, r1                  // Save the original stack pointer.
        clrrwi  r1, r1, 4               // Align the stack to 16 bytes.
        lis     r13, _SDA_BASE_@ha      // r13 points to the small data area.
        addi    r13, r13, _SDA_BASE_@l  //
        li      r0, 0                   // Zero the frame pointer.
        lwz     r3, 0(r9)               // and argc...
        addi    r4, r9, 4               // and argv ...
        mtlr    r0                      // Clear the link register.
        // Go to the musl dynamic linker entry point.
        bl      __dynlink
        cmpi    r4, 0, r3, 1            // Check for a 1.
        bne     r4, .                   // Stay here
        mtlr    r3                      // Set the link address...
        li      r3, 0
        blr                             // and go.
        .end    _start
        .size   _start, .-_start

