__asm__("\
.global _start \n\
_start: \n\
	mov r15, r4 \n\
	mov #-16, r0 \n\
	and r0, r15 \n\
	bsr __cstart \n\
	nop \n\
");
