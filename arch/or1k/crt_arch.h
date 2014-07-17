__asm__("\
.global _start \n\
.align  4 \n\
_start: \n\
	l.ori r3, r1, 0 \n\
	l.addi r2, r0, -8 \n\
	l.and r1, r1, r2 \n\
	l.addi r1, r1, -8 \n\
	l.jal __cstart \n\
	 l.ori r2, r0, 0 \n\
");
