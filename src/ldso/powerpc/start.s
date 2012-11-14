# FIXME : does not work, the small data array needs to be relocated.
# see elfspec_ppc.pdf, page 76-84
        .global _start
        .type   _start,@function
_start:
        mr      9, 1                  # Save the original stack pointer.
        clrrwi  1, 1, 4               # Align the stack to 16 bytes.
        lis     13, _SDA_BASE_@ha      # r13 points to the small data area.
        addi    13, 13, _SDA_BASE_@l
        li      0, 0                   # Zero the frame pointer.
        lwz     3, 0(9)               # and argc...
        addi    4, 9, 4               # and argv ...
        mtlr    0                      # Clear the link register.
        # Go to the musl dynamic linker entry point.
        bl      __dynlink
        cmpi    4, 0, 3, 1            # Check for a 1.
        bne     4, .                   # Stay here
        mtlr    3                      # Set the link address...
        li      3, 0
        blr                             # and go.
        .end    _start
        .size   _start, .-_start

