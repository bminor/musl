.global sigsetjmp
.global __sigsetjmp
.type sigsetjmp,@function
.type __sigsetjmp,@function
sigsetjmp:
__sigsetjmp:
	l.sfeq	r4, r0
	l.bf	plt(setjmp)

	l.sw    52(r3), r9
	l.sw    52+4+8(r3), r20

	l.jal	plt(setjmp)
	 l.ori   r20, r3, 0

	l.ori r4, r11, 0
	l.ori r3, r20, 0

	l.lwz   r9, 52(r3)

.hidden __sigsetjmp_tail
	l.j	plt(__sigsetjmp_tail)
	 l.lwz   r20, 52+4+8(r3)
