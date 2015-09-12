#ifdef __SH_FDPIC__

__asm__(
".text \n"
".global " START " \n"
START ": \n"
"	mov r8, r4 \n"
"	mova 1f, r0 \n"
"	mov.l 1f, r5 \n"
"	mov.l 1f+4, r6 \n"
"	add r0, r5 \n"
"	bsr __fdpic_fixup \n"
"	 add r0, r6 \n"
"	mov r0, r12 \n"
"	mov r15, r4 \n"
"	mov #-16, r0 \n"
"	bsr " START "_c \n"
"	 and r0, r15 \n"
".align 2 \n"
"1:	.long __ROFIXUP_LIST__@PCREL \n"
"	.long __ROFIXUP_END__@PCREL + 4 \n"
);

#include "fdpic_crt.h"

#else

__asm__(
".text \n"
".global " START " \n"
START ": \n"
"	mova 1f, r0 \n"
"	mov.l 1f, r5 \n"
"	add r0, r5 \n"
"	mov r15, r4 \n"
"	mov #-16, r0 \n"
"	and r0, r15 \n"
"	bsr " START "_c \n"
"	nop \n"
".align 2 \n"
".weak _DYNAMIC \n"
".hidden _DYNAMIC \n"
"1:	.long _DYNAMIC-. \n"
);

#endif

/* used by gcc for switching the FPU between single and double precision */
#ifdef SHARED
__attribute__((__visibility__("hidden")))
#endif
const unsigned long __fpscr_values[2] = { 0, 0x80000 };
